#include "curses_screen.h"

#include "menu_session.h"

#include <clocale>
#include <ncurses.h>

/* ncurses names these as macros. Screen methods use the same words. */
#undef getmaxyx
#undef addstr
#undef attrset
#undef erase
#undef getch
#undef move
#undef refresh
#undef timeout

namespace hallo {
namespace {

int mapKey(int key) {
    if (key == KEY_UP) return kKeyUp;
    if (key == KEY_DOWN) return kKeyDown;
    if (key == KEY_LEFT) return kKeyLeft;
    if (key == KEY_RIGHT) return kKeyRight;
    if (key == KEY_HOME) return kKeyHome;
    if (key == KEY_END) return kKeyEnd;
    if (key == KEY_ENTER) return kKeyEnter;
    if (key == KEY_BACKSPACE) return 127;
    return key;
}

int attrOf(int attr) {
    int bits = 0;
    if (attr & kAttrBold) bits |= A_BOLD;
#ifdef A_ITALIC
    if (attr & kAttrItalic) bits |= A_ITALIC;
#else
    if (attr & kAttrItalic) bits |= A_DIM;
#endif
    if (attr & kAttrReverse) bits |= A_REVERSE;
    if (attr & kAttrUnderline) bits |= A_UNDERLINE;
    return bits;
}

}  // namespace

CursesScreen::CursesScreen() {
    std::setlocale(LC_ALL, "");
    ::initscr();
    ::cbreak();
    ::noecho();
    ::keypad(stdscr, TRUE);
    open_ = true;
}

CursesScreen::~CursesScreen() {
    if (open_) ::endwin();
}

std::pair<int, int> CursesScreen::getmaxyx() const {
    return {getmaxy(stdscr), getmaxx(stdscr)};
}

void CursesScreen::erase() { werase(stdscr); }

void CursesScreen::addstr(int y, int x, const std::string &text, int attr) {
    wattrset(stdscr, attrOf(attr));
    mvwaddnstr(stdscr, y, x, text.c_str(), static_cast<int>(text.size()));
    wattrset(stdscr, A_NORMAL);
}

void CursesScreen::refresh() { wrefresh(stdscr); }
void CursesScreen::cursSet(int visibility) { curs_set(visibility); }
void CursesScreen::moveCursor(int y, int x) { wmove(stdscr, y, x); }
bool CursesScreen::hasTimeout() const { return true; }
void CursesScreen::timeoutMs(int milliseconds) { wtimeout(stdscr, milliseconds); }
int CursesScreen::getch() { return mapKey(wgetch(stdscr)); }
void CursesScreen::keypad(bool enabled) { ::keypad(stdscr, enabled ? TRUE : FALSE); }

int runOnCurses(const std::function<int(Screen &)> &body) {
    try {
        CursesScreen screen;
        return body(screen);
    } catch (const MenuScreenError &) {
        return 2;
    } catch (...) {
        return 1;
    }
}

}  // namespace hallo
