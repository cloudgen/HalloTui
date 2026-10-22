#ifndef HALLO_MENU_SESSION_H
#define HALLO_MENU_SESSION_H

#include "menu_model.h"

#include <functional>
#include <stdexcept>
#include <string>

namespace hallo {

class MenuSession {
public:
    MenuSession(std::string appName, std::string version, MenuPainter *painter,
                std::function<std::string()> onVersion, std::function<std::string()> onAbout,
                std::function<std::string(const std::string &)> onKind);

    std::string takeAction(const std::string &action);
    int run(Screen &screen);
    void leaveToFront();

    MenuModel model;
    std::string appName;
    std::string version;

private:
    /* nullptr when the one-second clock wait ended with no key. */
    bool waitKey(Screen &screen, int *key);
    MenuPainter *painter_;
    std::function<std::string()> onVersion_;
    std::function<std::string()> onAbout_;
    std::function<std::string(const std::string &)> onKind_;
};

class MenuScreenError : public std::runtime_error {
public:
    explicit MenuScreenError(const std::string &message);
};

}  // namespace hallo

#endif
