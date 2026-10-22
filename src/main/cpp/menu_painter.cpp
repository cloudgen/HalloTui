#include "menu_painter.h"

#include "menu_model.h"
#include "text_width.h"

#include <algorithm>
#include <ctime>
#include <unistd.h>

namespace hallo {
namespace {

std::string localClock() {
    const std::time_t now = std::time(nullptr);
    std::tm local {};
    localtime_r(&now, &local);
    char buffer[16];
    std::strftime(buffer, sizeof buffer, "%H:%M:%S", &local);
    return buffer;
}

std::string cwdPath() {
    char buffer[4096];
    if (::getcwd(buffer, sizeof buffer) == nullptr) return "";
    return buffer;
}

const std::vector<MenuRow> kMenuRows = {
    {1, "hello", "show a welcome message", "hello"},
    {3, "system-log", "view, clear, and the log folder", "system-log"},
    {4, "language", "display language for this menu", "language"},
    {8, "self-management", "version, about, and pip lifecycle", "self-management"},
    {9, "Exit", "leave", "exit"},
};

const std::vector<MenuRow> kSelfRows = {
    {82, "version", "show the installed version", "version"},
    {83, "about", "version and this computer", "about"},
    {84, "version-check", "compare this install with pip", "version-check"},
    {85, "self-update", "upgrade this package with pip", "self-update"},
    {86, "self-uninstall", "remove this package with pip", "self-uninstall"},
    {87, "self-install", "install this package with pip", "self-install"},
    {0, "Back", "return to the main menu", "back"},
};

const std::vector<MenuRow> kLogRows = {
    {31, "view-log", "list a log file and show it", "view-log"},
    {32, "clear-log", "empty one log file", "clear-log"},
    {33, "log-folder", "show the log folder", "log-folder"},
    {0, "Back", "return to the main menu", "back"},
};

std::vector<std::string> splitLines(const std::string &text) {
    std::vector<std::string> lines;
    std::string current;
    for (char ch : text) {
        if (ch == '\n') {
            lines.push_back(current);
            current.clear();
        } else {
            current.push_back(ch);
        }
    }
    if (!current.empty() || lines.empty()) lines.push_back(current);
    if (lines.empty()) lines.emplace_back("");
    return lines;
}

}  // namespace

MenuPainter::MenuPainter() : language_() { clockText = localClock; }

MenuPainter::MenuPainter(std::string home) : language_(std::move(home)) { clockText = localClock; }

MenuLanguage &MenuPainter::language() { return language_; }
const MenuLanguage &MenuPainter::language() const { return language_; }

const std::vector<MenuRow> &MenuPainter::menuRows() { return kMenuRows; }
const std::vector<MenuRow> &MenuPainter::selfRows() { return kSelfRows; }
const std::vector<MenuRow> &MenuPainter::logRows() { return kLogRows; }

std::vector<MenuRow> MenuPainter::rowsFor(const std::string &layer) const {
    if (layer == "self") return kSelfRows;
    if (layer == "log") return kLogRows;
    return kMenuRows;
}

std::vector<MenuRow> MenuPainter::displayRows(const std::string &layer) const {
    std::vector<MenuRow> built;
    if (layer == "lang") {
        for (const LanguageCode &item : MenuLanguage::kCodes) {
            built.push_back(MenuRow{item.number, item.endonym, language_.langLong(item.endonym),
                                     std::string("set-") + item.code});
        }
        built.push_back(MenuRow{0, language_.rowShort("back", "Back"),
                                 language_.rowLong("back", "return to the main menu"), "back"});
        return built;
    }
    for (const MenuRow &row : rowsFor(layer)) {
        built.push_back(MenuRow{row.number, language_.rowShort(row.kind, row.shortText),
                                 language_.rowLong(row.kind, row.explain), row.kind});
    }
    return built;
}

std::string MenuPainter::pathLabel() const { return language_.pathLabel(); }

std::string MenuPainter::pathLine(int placeable) const {
    const std::string prefix = pathLabel() + ": ";
    const std::string left = prefix + cwdPath();
    const std::string right = clockText ? clockText() : localClock();
    const int gap = 2;
    const int leftWidth = displayWidth(left);
    const int rightWidth = displayWidth(right);
    const int prefixWidth = displayWidth(prefix);
    if (placeable < 0) return left + std::string(static_cast<std::size_t>(gap), ' ') + right;
    if (placeable >= leftWidth + gap + rightWidth) {
        const int spaces = placeable - leftWidth - rightWidth;
        return left + std::string(static_cast<std::size_t>(spaces), ' ') + right;
    }
    if (placeable >= leftWidth) return left;
    if (placeable <= prefixWidth) return clipColumns(prefix, placeable);
    return prefix + clipColumnsRight(cwdPath(), placeable - prefixWidth);
}

std::string MenuPainter::boardTitle(const MenuModel &model) const {
    if (model.phase == "result") return "result";
    if (model.layer == "self") return language_.text("title_self");
    if (model.layer == "log") return language_.text("title_log");
    if (model.layer == "lang") return language_.text("title_language");
    return language_.text("title_main");
}

std::vector<RowPart> MenuPainter::rowParts(const std::vector<MenuRow> &rows) const {
    int numberWidth = 1;
    int verbWidth = 0;
    for (const MenuRow &row : rows) {
        const std::string text = std::to_string(row.number);
        numberWidth = std::max(numberWidth, static_cast<int>(text.size()));
        verbWidth = std::max(verbWidth, displayWidth(row.shortText));
    }
    std::vector<RowPart> parts;
    for (const MenuRow &row : rows) {
        const std::string text = std::to_string(row.number);
        RowPart part;
        part.numberField = std::string(static_cast<std::size_t>(numberWidth - static_cast<int>(text.size())), ' ') +
                           text + ". ";
        part.shortText = row.shortText;
        part.verbPad = std::string(static_cast<std::size_t>(verbWidth - displayWidth(row.shortText)), ' ');
        part.explain = row.explain;
        parts.push_back(part);
    }
    return parts;
}

std::vector<std::string> MenuPainter::formatRows(const std::vector<MenuRow> &rows) const {
    std::vector<std::string> lines;
    for (const RowPart &part : rowParts(rows)) {
        lines.push_back(part.numberField + part.shortText + part.verbPad + ": " + part.explain);
    }
    return lines;
}

bool MenuPainter::screenCanHold(int height, int width) const {
    return height >= kMinHeight && width - 1 >= kMinPlaceable;
}

bool MenuPainter::resultOverflow(int height, int lineCount) const {
    return lineCount > std::max(1, height - 3);
}

int MenuPainter::resultRoom(int height, int lineCount) const {
    if (resultOverflow(height, lineCount)) return std::max(1, height - 4);
    return std::max(1, height - 3);
}

void MenuPainter::put(Screen &screen, int y, int x, const std::string &text, int attr) const {
    const auto size = screen.getmaxyx();
    if (y < 0 || y >= size.first || x >= size.second - 1) return;
    const std::string clipped = clipColumns(text, std::max(0, size.second - x - 1));
    if (!clipped.empty()) screen.addstr(y, x, clipped, attr);
}

std::string MenuPainter::banner(const MenuModel &model) const {
    if (!model.notice.empty()) return model.notice;
    return model.error;
}

std::pair<std::string, int> MenuPainter::inputField(const MenuModel &model, int inner) const {
    const std::string prefix = std::string(" ") + kInputMark;
    const bool focused = model.focus == "input";
    int cursor = std::min(std::max(model.cursor, 0), static_cast<int>(model.buffer.size()));
    const int reserve = static_cast<int>(prefix.size()) + (focused ? 1 : 0);
    const int textRoom = std::max(0, inner - reserve);
    int start = 0;
    if (textRoom && cursor > textRoom) start = cursor - textRoom;
    if (textRoom && start > std::max(0, static_cast<int>(model.buffer.size()) - textRoom)) {
        start = std::max(0, static_cast<int>(model.buffer.size()) - textRoom);
    }
    const std::string shown = textRoom ? model.buffer.substr(static_cast<std::size_t>(start),
                                                              static_cast<std::size_t>(textRoom))
                                       : "";
    const int rel = std::max(0, std::min(static_cast<int>(shown.size()), cursor - start));
    std::string body = shown;
    int caretAt = -1;
    if (focused && inner >= reserve) {
        body = shown.substr(0, static_cast<std::size_t>(rel)) + kCaretBlock +
               shown.substr(static_cast<std::size_t>(rel));
        caretAt = static_cast<int>(prefix.size()) + rel;
    }
    std::string field = prefix + body;
    auto codePoints = [](const std::string &text) {
        int count = 0;
        for (unsigned char ch : text) {
            if ((ch & 0xC0) != 0x80) ++count;
        }
        return count;
    };
    auto clipPoints = [&](const std::string &text, int count) {
        if (count <= 0) return std::string();
        std::string kept;
        int seen = 0;
        for (std::size_t index = 0; index < text.size();) {
            if (seen == count) break;
            unsigned char lead = static_cast<unsigned char>(text[index]);
            std::size_t need = 1;
            if ((lead & 0xE0) == 0xC0) need = 2;
            else if ((lead & 0xF0) == 0xE0) need = 3;
            else if ((lead & 0xF8) == 0xF0) need = 4;
            if (index + need > text.size()) need = 1;
            kept.append(text, index, need);
            index += need;
            ++seen;
        }
        return kept;
    };
    const int have = codePoints(field);
    if (have < inner) field.append(static_cast<std::size_t>(inner - have), ' ');
    if (have > inner) field = clipPoints(field, inner);
    if (caretAt >= inner) caretAt = -1;
    return {field, caretAt};
}

std::string MenuPainter::statusLine(const std::string &appName, const std::string &version,
                                    const std::string &title) const {
    return std::string("  ") + appName + " " + version + "  " + kFrameVert + "  " + title + "  " +
           kFrameVert + "  " + kInputHint;
}

void MenuPainter::paintBox(Screen &screen, const MenuModel &model, int top, const std::string &appName,
                           const std::string &version, const std::string &title) const {
    const auto size = screen.getmaxyx();
    const int placeable = size.second - 1;
    if (top < 0 || placeable < 2) return;
    const int inner = placeable - 2;
    const auto field = inputField(model, inner);
    std::string horiz;
    for (int i = 0; i < inner; ++i) horiz += kFrameHoriz;
    const std::string lines[3] = {
        std::string(kFrameTopLeft) + horiz + kFrameTopRight,
        std::string(kFrameVert) + field.first + kFrameVert,
        std::string(kFrameBottomLeft) + horiz + kFrameBottomRight,
    };
    for (int offset = 0; offset < 3; ++offset) put(screen, top + offset, 0, lines[offset], 0);
    put(screen, top + kFrameRows, 0, statusLine(appName, version, title), 0);
    if (field.second >= 0) screen.moveCursor(top + 1, 1 + field.second);
}

void MenuPainter::paint(Screen &screen, MenuModel &model, const std::string &appName,
                        const std::string &version) {
    screen.erase();
    const std::string title = boardTitle(model);
    if (model.phase == "result") {
        put(screen, 0, 0, appName, kAttrBold);
        int cursor = displayWidth(appName);
        put(screen, 0, cursor, " (", 0);
        cursor += 2;
        put(screen, 0, cursor, version, kAttrItalic);
        cursor += displayWidth(version);
        put(screen, 0, cursor, ") — " + title, 0);
        const std::vector<std::string> lines = splitLines(model.resultText);
        const int height = screen.getmaxyx().first;
        const int room = resultRoom(height, static_cast<int>(lines.size()));
        const int maxOffset = std::max(0, static_cast<int>(lines.size()) - room);
        if (model.resultOffset > maxOffset) model.resultOffset = maxOffset;
        if (model.resultOffset < 0) model.resultOffset = 0;
        int y = 2;
        for (int index = model.resultOffset;
             index < static_cast<int>(lines.size()) && index < model.resultOffset + room; ++index) {
            if (y >= height - 1) break;
            put(screen, y, 0, lines[static_cast<std::size_t>(index)], 0);
            ++y;
        }
        if (resultOverflow(height, static_cast<int>(lines.size()))) {
            put(screen, height - 2, 0, "Up/Down scrolls this page.", kAttrItalic);
            put(screen, height - 1, 0, "Press a key to return to the main menu.", kAttrItalic);
        } else {
            int hintY = y + 1;
            if (hintY >= height) hintY = height - 1;
            put(screen, hintY, 0, "Press a key to return to the main menu.", kAttrItalic);
        }
        screen.refresh();
        return;
    }
    const auto size = screen.getmaxyx();
    put(screen, 0, 0, pathLine(std::max(0, size.second - 1)), 0);
    const int boxTop = size.first - kChromeRows;
    const std::string note = banner(model);
    int stop = boxTop;
    if (!note.empty() && boxTop >= 4) stop = boxTop - 1;
    int y = 2;
    const std::vector<RowPart> parts = rowParts(model.rows());
    if (y < stop) {
        for (int idx = 0; idx < static_cast<int>(parts.size()); ++idx) {
            if (y >= stop) break;
            const RowPart &part = parts[static_cast<std::size_t>(idx)];
            const bool selected = idx == model.index;
            int attr = 0;
            if (selected && model.focus == "list") attr = kAttrReverse;
            else if (selected) attr = kAttrUnderline;
            put(screen, y, 0, part.numberField, attr);
            const int verbX = displayWidth(part.numberField);
            put(screen, y, verbX, part.shortText, kAttrBold | attr);
            const int colonX = verbX + displayWidth(part.shortText);
            put(screen, y, colonX, part.verbPad + ": ", attr);
            const int explainX = colonX + displayWidth(part.verbPad) + 2;
            put(screen, y, explainX, part.explain, kAttrItalic);
            ++y;
        }
    }
    if (!note.empty() && boxTop >= 4) put(screen, boxTop - 1, 0, note, kAttrBold);
    paintBox(screen, model, boxTop, appName, version, title);
    screen.cursSet(0);
    screen.refresh();
}

void MenuPainter::paintPrompt(
    Screen &screen, MenuModel &model, const std::string &title, const std::vector<std::string> &lines,
    bool pinLast, const std::string &appName, const std::string &version,
    const std::function<std::vector<std::string>(const std::vector<std::string> &, int, bool)>
        &visibleLines) {
    screen.erase();
    put(screen, 0, 0, appName, kAttrBold);
    int cursor = displayWidth(appName);
    put(screen, 0, cursor, " (", 0);
    cursor += 2;
    put(screen, 0, cursor, version, kAttrItalic);
    cursor += displayWidth(version);
    put(screen, 0, cursor, ") — " + title, 0);
    const int height = screen.getmaxyx().first;
    const int boxTop = height - kChromeRows;
    const std::string note = banner(model);
    int stop = boxTop;
    if (!note.empty() && boxTop >= 4) stop = boxTop - 1;
    const std::vector<std::string> shown = visibleLines(lines, stop - 2, pinLast);
    int y = 2;
    for (const std::string &line : shown) {
        if (y >= stop) break;
        put(screen, y, 0, line, 0);
        ++y;
    }
    if (!note.empty() && boxTop >= 4) put(screen, boxTop - 1, 0, note, kAttrBold);
    paintBox(screen, model, boxTop, appName, version, title);
    screen.cursSet(0);
    screen.refresh();
}

}  // namespace hallo
