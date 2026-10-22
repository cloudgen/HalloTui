#ifndef HALLO_CLI_H
#define HALLO_CLI_H

#include "about_page.h"
#include "self_manage.h"
#include "tui.h"

#include <string>
#include <vector>

namespace hallo {

class Cli {
public:
    static bool stdoutIsTty();
    static bool opensTextScreen(const std::vector<std::string> &argv, bool tty);

    Cli();
    int run(const std::vector<std::string> &argv);
    int reportError(const std::string &message, const std::string &next) const;

    AboutPage about;
    SelfManage selfManage;
    Tui tui;

private:
    std::string helpText() const;
    int unknownVerb(const std::string &token) const;
};

}  // namespace hallo

#endif
