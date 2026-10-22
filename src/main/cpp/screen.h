#ifndef HALLO_SCREEN_H
#define HALLO_SCREEN_H

#include <string>
#include <utility>

namespace hallo {

/* The painter draws through this surface. A test screen records the calls.
   The terminal screen is ncurses. Key numbers match ncurses KEY_* values. */

constexpr int kKeyUp = 259;
constexpr int kKeyDown = 258;
constexpr int kKeyLeft = 260;
constexpr int kKeyRight = 261;
constexpr int kKeyHome = 262;
constexpr int kKeyEnd = 360;
constexpr int kKeyEnter = 343;
constexpr int kKeyBackspace = 263;

constexpr int kAttrBold = 1 << 0;
constexpr int kAttrItalic = 1 << 1;
constexpr int kAttrReverse = 1 << 2;
constexpr int kAttrUnderline = 1 << 3;

class Screen {
public:
    virtual ~Screen() = default;
    virtual std::pair<int, int> getmaxyx() const = 0;
    virtual void erase() = 0;
    virtual void addstr(int y, int x, const std::string &text, int attr) = 0;
    virtual void refresh() = 0;
    virtual void cursSet(int visibility) = 0;
    virtual void moveCursor(int y, int x) = 0;
    virtual bool hasTimeout() const = 0;
    virtual void timeoutMs(int milliseconds) = 0;
    virtual int getch() = 0;
    virtual void keypad(bool enabled) = 0;
};

}  // namespace hallo

#endif
