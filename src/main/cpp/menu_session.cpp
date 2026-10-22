#include "menu_session.h"

#include <stdexcept>

namespace hallo {

MenuScreenError::MenuScreenError(const std::string &message) : std::runtime_error(message) {}

MenuSession::MenuSession(std::string appNameIn, std::string versionIn, MenuPainter *painter,
                         std::function<std::string()> onVersion,
                         std::function<std::string()> onAbout,
                         std::function<std::string(const std::string &)> onKind)
    : model(painter),
      appName(std::move(appNameIn)),
      version(std::move(versionIn)),
      painter_(painter),
      onVersion_(std::move(onVersion)),
      onAbout_(std::move(onAbout)),
      onKind_(std::move(onKind)) {}

void MenuSession::leaveToFront() { model.showFront(); }

bool MenuSession::waitKey(Screen &screen, int *key) {
    const bool showClock = model.phase != "result" &&
                           (model.layer == "front" || model.layer == "self" || model.layer == "log" ||
                            model.layer == "lang");
    if (screen.hasTimeout()) screen.timeoutMs(showClock ? 1000 : -1);
    const int read = screen.getch();
    if (read == -1 && showClock && screen.hasTimeout()) return false;
    *key = read;
    return true;
}

int MenuSession::run(Screen &screen) {
    screen.keypad(true);
    const auto size = screen.getmaxyx();
    if (!painter_->screenCanHold(size.first, size.second)) {
        throw MenuScreenError("The text screen is too small for the menu and the input box.");
    }
    while (true) {
        painter_->paint(screen, model, appName, version);
        const auto next = screen.getmaxyx();
        int key = 0;
        if (!waitKey(screen, &key)) continue;
        const std::string action = model.applyKey(key, next.first);
        if (takeAction(action) == "exit") return 0;
    }
}

std::string MenuSession::takeAction(const std::string &action) {
    if (action == "exit") return "exit";
    if (action.compare(0, 4, "set-") == 0) {
        const std::string message = painter_->language().save(action.substr(4));
        model.showFront();
        model.notice = message;
        return "stay";
    }
    if (action == "version") {
        model.showResult(onVersion_ ? onVersion_() : "");
        return "stay";
    }
    if (action == "about") {
        model.showResult(onAbout_ ? onAbout_() : "");
        return "stay";
    }
    if (!action.empty() && onKind_) {
        const std::string outcome = onKind_(action);
        if (outcome == "leave") return "exit";
        if (!outcome.empty()) model.showResult(outcome);
    }
    return "stay";
}

}  // namespace hallo
