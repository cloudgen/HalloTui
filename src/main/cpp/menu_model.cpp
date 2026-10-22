#include "menu_model.h"

#include <algorithm>
#include <cctype>

namespace hallo {
namespace {

bool isEnter(int key) { return key == 10 || key == 13 || key == kKeyEnter; }

bool anyBoard(const std::string &token, std::string *kind) {
    static const char *kTokens[][2] = {
        {"help", "help"},
        {"version", "version"},
        {"about", "about"},
        {"hello", "hello"},
        {"self-install", "self-install"},
        {"version-check", "version-check"},
        {"self-update", "self-update"},
        {"self-uninstall", "self-uninstall"},
        {"self-management", "self-management"},
        {"system-log", "system-log"},
        {"view-log", "view-log"},
        {"clear-log", "clear-log"},
        {"log-folder", "log-folder"},
    };
    for (const auto &row : kTokens) {
        if (token == row[0]) {
            *kind = row[1];
            return true;
        }
    }
    return false;
}

}  // namespace

MenuModel::MenuModel(MenuPainter *painterIn) : painter(painterIn) {}

std::vector<MenuRow> MenuModel::rows() const {
    return painter->displayRows(layer);
}

bool MenuModel::hasHighlight() const {
    const std::vector<MenuRow> board = rows();
    return index >= 0 && index < static_cast<int>(board.size());
}

MenuRow MenuModel::highlightedRow() const { return rows().at(static_cast<std::size_t>(index)); }

void MenuModel::showResult(const std::string &text) {
    resultText = text;
    resultOffset = 0;
    phase = "result";
    layer = "front";
    index = 0;
    buffer.clear();
    cursor = 0;
    focus = "list";
    error.clear();
}

void MenuModel::showFront() {
    phase = "board";
    layer = "front";
    index = 0;
    buffer.clear();
    cursor = 0;
    focus = "list";
    error.clear();
    resultOffset = 0;
}

std::string MenuModel::applyKey(int key, int screenHeight) {
    notice.clear();
    if (key == 27) {
        if (phase == "result" || layer != "front") showFront();
        return "";
    }
    if (key == -1) {
        if (phase == "result" || layer != "front") {
            showFront();
            return "";
        }
        return "exit";
    }
    if (phase == "result") {
        int lineCount = 1;
        if (!resultText.empty()) {
            lineCount = 1 + static_cast<int>(std::count(resultText.begin(), resultText.end(), '\n'));
            if (resultText.back() == '\n') --lineCount;
        }
        if (painter->resultOverflow(screenHeight, lineCount) && (key == kKeyUp || key == 'k')) {
            scrollResult(-1, screenHeight);
            return "";
        }
        if (painter->resultOverflow(screenHeight, lineCount) && (key == kKeyDown || key == 'j')) {
            scrollResult(1, screenHeight);
            return "";
        }
        showFront();
        return "";
    }
    if (key == kKeyUp || key == kKeyDown || (focus == "list" && (key == 'k' || key == 'j'))) {
        arrow(key);
        return "";
    }
    if (key == kKeyLeft || key == kKeyRight || key == kKeyHome || key == kKeyEnd) {
        slide(key);
        return "";
    }
    if (key == kKeyBackspace || key == 127 || key == 8) {
        backspace();
        return "";
    }
    if (isEnter(key)) return commit();
    if (key >= 32 && key < 127) {
        insertChar(static_cast<char>(key));
        return "";
    }
    error = painter->language().text("invalid");
    return "";
}

void MenuModel::scrollResult(int delta, int screenHeight) {
    int lineCount = 1;
    if (!resultText.empty()) {
        lineCount = static_cast<int>(std::count(resultText.begin(), resultText.end(), '\n')) + 1;
    }
    const int room = painter->resultRoom(screenHeight, lineCount);
    const int maxOffset = std::max(0, lineCount - room);
    resultOffset = std::min(std::max(0, resultOffset + delta), maxOffset);
}

void MenuModel::arrow(int key) {
    const int last = static_cast<int>(rows().size()) - 1;
    const bool down = key == kKeyDown || key == 'j';
    if (focus == "input") {
        focus = "list";
        index = down ? 0 : last;
        return;
    }
    if (down) {
        if (index < last) {
            ++index;
        } else {
            focus = "input";
            cursor = static_cast<int>(buffer.size());
        }
        return;
    }
    if (index > 0) {
        --index;
        return;
    }
    focus = "input";
    cursor = static_cast<int>(buffer.size());
}

void MenuModel::slide(int key) {
    if (focus != "input") return;
    if (key == kKeyLeft) cursor = std::max(0, cursor - 1);
    else if (key == kKeyRight) cursor = std::min(static_cast<int>(buffer.size()), cursor + 1);
    else if (key == kKeyHome) cursor = 0;
    else if (key == kKeyEnd) cursor = static_cast<int>(buffer.size());
}

void MenuModel::insertChar(char ch) {
    if (focus != "input") {
        focus = "input";
        cursor = static_cast<int>(buffer.size());
    }
    cursor = std::min(std::max(cursor, 0), static_cast<int>(buffer.size()));
    buffer.insert(buffer.begin() + cursor, ch);
    ++cursor;
}

void MenuModel::backspace() {
    if (focus != "input") {
        focus = "input";
        cursor = static_cast<int>(buffer.size());
    }
    if (cursor <= 0 || buffer.empty()) {
        cursor = 0;
        return;
    }
    cursor = std::min(cursor, static_cast<int>(buffer.size()));
    buffer.erase(buffer.begin() + cursor - 1);
    --cursor;
}

void MenuModel::reject() {
    error = painter->language().text("invalid");
    focus = "input";
    cursor = static_cast<int>(buffer.size());
}

std::string MenuModel::commit() {
    std::string token = buffer;
    while (!token.empty() && std::isspace(static_cast<unsigned char>(token.front()))) token.erase(token.begin());
    while (!token.empty() && std::isspace(static_cast<unsigned char>(token.back()))) token.pop_back();
    buffer.clear();
    cursor = 0;
    if (token.empty()) {
        const MenuRow row = rows().at(static_cast<std::size_t>(index));
        return activate(row.number, row.kind);
    }
    bool digits = !token.empty();
    for (char ch : token) {
        if (!std::isdigit(static_cast<unsigned char>(ch))) digits = false;
    }
    if (digits) return activateNumber(std::stoi(token));
    for (const MenuRow &row : rows()) {
        if (token == row.shortText) return activate(row.number, row.kind);
    }
    const std::string mapped = painter->language().tokenKind(token, layer);
    if (!mapped.empty()) return activate(0, mapped);
    std::string kind;
    if (anyBoard(token, &kind)) return activate(0, kind);
    reject();
    return "";
}

std::string MenuModel::activateNumber(int number) {
    for (const MenuRow &row : rows()) {
        if (row.number == number) return activate(number, row.kind);
    }
    reject();
    return "";
}

std::string MenuModel::activate(int number, const std::string &kind) {
    (void)number;
    error.clear();
    focus = "list";
    cursor = 0;
    if (kind == "exit") return "exit";
    if (kind == "back") {
        layer = "front";
        index = 0;
        return "";
    }
    if (kind == "self-management" || kind == "system-log" || kind == "language") {
        if (kind == "self-management") layer = "self";
        else if (kind == "system-log") layer = "log";
        else layer = "lang";
        index = 0;
        focus = "list";
        buffer.clear();
        cursor = 0;
        return "";
    }
    return kind;
}

}  // namespace hallo
