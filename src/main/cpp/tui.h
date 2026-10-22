#ifndef HALLO_TUI_H
#define HALLO_TUI_H

#include "about_page.h"
#include "menu_painter.h"
#include "self_manage.h"
#include "system_log.h"

#include <string>
#include <vector>

namespace hallo {

class Tui {
public:
    Tui(std::string appName, std::string version, AboutPage *about, SelfManage *selfManage,
        std::string logFolder);

    std::string helloText() const;
    std::string helpOnScreen() const;
    std::string aboutText() const;
    std::vector<std::string> frontLines() const;
    std::vector<std::string> selfLines() const;
    std::vector<std::string> logLines() const;
    std::vector<std::string> languageLines() const;
    int openTextMenu(bool stdoutIsTty);
    int openAbout(bool stdoutIsTty);
    MenuPainter &painter();
    SystemLog &systemLog();

    std::string appName;
    std::string version;

private:
    std::string onKind(const std::string &kind, class MenuSession *session, class Screen *screen);
    AboutPage *about_;
    SelfManage *selfManage_;
    MenuPainter painter_;
    SystemLog systemLog_;
};

}  // namespace hallo

#endif
