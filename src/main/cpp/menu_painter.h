#ifndef HALLO_MENU_PAINTER_H
#define HALLO_MENU_PAINTER_H

#include "menu_language.h"
#include "screen.h"

#include <functional>
#include <string>
#include <tuple>
#include <vector>

namespace hallo {

struct MenuRow {
    int number;
    std::string shortText;
    std::string explain;
    std::string kind;
};

struct RowPart {
    std::string numberField;
    std::string shortText;
    std::string verbPad;
    std::string explain;
};

class MenuModel;

class MenuPainter {
public:
    static constexpr const char *kInputMark = "> ";
    static constexpr const char *kInputHint = "Up/Down  •  Enter";
    static constexpr const char *kPathLabelEn = "Path";
    static constexpr const char *kPathLabelZhHant = "路徑";
    static constexpr const char *kFrameTopLeft = "╭";
    static constexpr const char *kFrameTopRight = "╮";
    static constexpr const char *kFrameBottomLeft = "╰";
    static constexpr const char *kFrameBottomRight = "╯";
    static constexpr const char *kFrameHoriz = "─";
    static constexpr const char *kFrameVert = "│";
    static constexpr const char *kCaretBlock = "█";
    static constexpr int kFrameRows = 3;
    static constexpr int kFooterRows = 1;
    static constexpr int kChromeRows = kFrameRows + kFooterRows;
    static constexpr int kMinMenuAbove = 3;
    static constexpr int kMinHeight = kMinMenuAbove + kChromeRows;
    static constexpr int kMinPlaceable = 2 + 1 + 2 + 1;

    /* No argument reads HOME. An empty string stays empty and cannot save. */
    MenuPainter();
    explicit MenuPainter(std::string home);

    MenuLanguage &language();
    const MenuLanguage &language() const;

    std::function<std::string()> clockText;

    std::vector<MenuRow> rowsFor(const std::string &layer) const;
    std::vector<MenuRow> displayRows(const std::string &layer) const;
    std::string pathLabel() const;
    std::string pathLine(int placeable = -1) const;
    std::string boardTitle(const MenuModel &model) const;
    std::vector<RowPart> rowParts(const std::vector<MenuRow> &rows) const;
    std::vector<std::string> formatRows(const std::vector<MenuRow> &rows) const;
    bool screenCanHold(int height, int width) const;
    bool resultOverflow(int height, int lineCount) const;
    int resultRoom(int height, int lineCount) const;
    void paint(Screen &screen, MenuModel &model, const std::string &appName,
               const std::string &version);
    void paintPrompt(Screen &screen, MenuModel &model, const std::string &title,
                     const std::vector<std::string> &lines, bool pinLast,
                     const std::string &appName, const std::string &version,
                     const std::function<std::vector<std::string>(
                         const std::vector<std::string> &, int, bool)> &visibleLines);

    static const std::vector<MenuRow> &menuRows();
    static const std::vector<MenuRow> &selfRows();
    static const std::vector<MenuRow> &logRows();

private:
    void put(Screen &screen, int y, int x, const std::string &text, int attr) const;
    std::string banner(const MenuModel &model) const;
    std::pair<std::string, int> inputField(const MenuModel &model, int inner) const;
    std::string statusLine(const std::string &appName, const std::string &version,
                           const std::string &title) const;
    void paintBox(Screen &screen, const MenuModel &model, int top, const std::string &appName,
                  const std::string &version, const std::string &title) const;

    MenuLanguage language_;
};

}  // namespace hallo

#endif
