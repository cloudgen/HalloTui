#ifndef HALLO_CURSES_SCREEN_H
#define HALLO_CURSES_SCREEN_H

#include "screen.h"

#include <functional>

namespace hallo {

class CursesScreen : public Screen {
public:
    CursesScreen();
    ~CursesScreen() override;
    std::pair<int, int> getmaxyx() const override;
    void erase() override;
    void addstr(int y, int x, const std::string &text, int attr) override;
    void refresh() override;
    void cursSet(int visibility) override;
    void moveCursor(int y, int x) override;
    bool hasTimeout() const override;
    void timeoutMs(int milliseconds) override;
    int getch() override;
    void keypad(bool enabled) override;

private:
    bool open_ = false;
};

int runOnCurses(const std::function<int(Screen &)> &body);

}  // namespace hallo

#endif
