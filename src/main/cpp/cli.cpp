#include "cli.h"

#include "project.h"

#include <iostream>
#include <unistd.h>

namespace hallo {
namespace {

struct Args {
    std::string verb;
    bool force = false;
    bool help = false;
    bool version = false;
    bool bad = false;
    std::string badToken;
};

Args parseArgs(const std::vector<std::string> &argv) {
    Args args;
    for (const std::string &token : argv) {
        if (token == "--help" || token == "-h" || token == "help") {
            args.help = true;
            continue;
        }
        if (token == "--version") {
            args.version = true;
            continue;
        }
        if (token == "--force") {
            args.force = true;
            continue;
        }
        if (!token.empty() && token[0] == '-') {
            args.bad = true;
            args.badToken = token;
            return args;
        }
        if (!args.verb.empty()) {
            args.bad = true;
            args.badToken = token;
            return args;
        }
        args.verb = token;
    }
    return args;
}

}  // namespace

bool Cli::stdoutIsTty() { return ::isatty(STDOUT_FILENO) == 1; }

bool Cli::opensTextScreen(const std::vector<std::string> &argv, bool tty) {
    for (const std::string &token : argv) {
        if (token == "--help" || token == "-h" || token == "--version") return false;
    }
    if (!tty) return false;
    std::string verb;
    bool sawForce = false;
    for (const std::string &token : argv) {
        if (token == "--force") {
            sawForce = true;
            continue;
        }
        if (!token.empty() && token[0] == '-') return false;
        if (verb.empty()) {
            verb = token;
            continue;
        }
        return false;
    }
    if (sawForce && verb != "self-uninstall") return false;
    if (verb == "about") return true;
    return verb.empty();
}

Cli::Cli()
    : about(kAppName, kVersion, kConsoleName, kAuthorName, kLastUpdate, kHomepage, kBasicUsage),
      selfManage(kAppName, kVersion),
      tui(kAppName, kVersion, &about, &selfManage, "") {}

std::string Cli::helpText() const {
    return std::string("usage: ") + kConsoleName +
           " [verb]\n"
           "\n" +
           kAppName +
           " — print a welcome message.\n"
           "With no arguments on a terminal, opens the text menu.\n"
           "With no arguments and no terminal, prints this help and stops.\n"
           "Product verbs: help, version, about, hello, self-install,\n"
           "version-check, self-update, self-uninstall.\n"
           "help prints this usage. version prints the installed version.\n"
           "about prints a page. hello prints a welcome line and does not\n"
           "draw the menu. Menu row 1 hello shows that line on the menu.\n"
           "This program does not join media or list files in this folder.\n"
           "version-check runs: python -m pip index versions " +
           std::string(kAppName) +
           "\n"
           "self-update runs: python -m pip install --upgrade " +
           std::string(kAppName) +
           "\n"
           "self-install runs: python -m pip install " +
           std::string(kAppName) +
           "\n"
           "self-uninstall runs: python -m pip uninstall -y " +
           std::string(kAppName) +
           "\n"
           "and needs --force. Empty arguments do not install or update.\n";
}

int Cli::reportError(const std::string &message, const std::string &next) const {
    std::cerr << "ERROR: " << message << "\n";
    std::cerr << "   Next: " << next << "\n";
    return 1;
}

int Cli::unknownVerb(const std::string &token) const {
    return reportError("Unknown verb '" + token + "'.",
                       std::string(kConsoleName) +
                           " help — verbs: help, version, about, hello, self-install, "
                           "version-check, self-update, self-uninstall");
}

int Cli::run(const std::vector<std::string> &argv) {
    const Args args = parseArgs(argv);
    if (args.version) {
        std::cout << kAppName << " " << kVersion << "\n";
        return 0;
    }
    if (args.help) {
        std::cout << helpText();
        return 0;
    }
    if (args.bad) return unknownVerb(args.badToken);
    if (args.force && args.verb != "self-uninstall") {
        return reportError("--force is only for self-uninstall.",
                           std::string(kConsoleName) + " self-uninstall --force");
    }
    if (args.verb == "about") {
        if (stdoutIsTty()) return tui.openAbout(true);
        std::cout << about.frameworkAbout() << "\n";
        return 0;
    }
    if (args.verb == "hello") {
        std::cout << tui.helloText() << "\n";
        return 0;
    }
    if (args.verb == "version") {
        std::cout << selfManage.localVersion() << "\n";
        return 0;
    }
    if (args.verb == "self-uninstall") {
        if (!args.force) {
            return reportError("self-uninstall removes this package with pip.",
                               std::string(kConsoleName) + " self-uninstall --force");
        }
        return selfManage.emit(args.verb);
    }
    if (args.verb == "version-check" || args.verb == "self-update" || args.verb == "self-install") {
        return selfManage.emit(args.verb);
    }
    if (!args.verb.empty()) return unknownVerb(args.verb);
    if (!stdoutIsTty()) {
        std::cout << helpText();
        return 0;
    }
    const int code = tui.openTextMenu(true);
    return code == 0 ? 0 : 1;
}

}  // namespace hallo
