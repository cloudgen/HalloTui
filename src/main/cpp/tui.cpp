#include "tui.h"

#include "curses_screen.h"
#include "menu_session.h"
#include "project.h"

#include <cstdio>
#include <memory>
#include <sstream>

namespace hallo {
namespace {

std::vector<std::string> withPath(const MenuPainter &painter, const std::vector<MenuRow> &rows) {
    std::vector<std::string> lines;
    lines.push_back(painter.pathLine());
    const std::vector<std::string> body = painter.formatRows(rows);
    lines.insert(lines.end(), body.begin(), body.end());
    return lines;
}

std::string readLine(MenuPainter &painter, MenuModel &model, Screen &screen,
                     const std::vector<std::string> &lines, const std::string &title,
                     const std::string &appName, const std::string &version) {
    model.buffer.clear();
    model.cursor = 0;
    model.focus = "input";
    auto visible = [](const std::vector<std::string> &all, int room, bool pinLast) {
        std::vector<std::string> shown;
        if (room < 1) return shown;
        if (static_cast<int>(all.size()) <= room) return all;
        if (!pinLast) {
            return std::vector<std::string>(all.end() - room, all.end());
        }
        if (room == 1) return std::vector<std::string>{all.back()};
        shown.insert(shown.end(), all.begin(), all.begin() + (room - 2));
        shown.push_back("…");
        shown.push_back(all.back());
        return shown;
    };
    while (true) {
        painter.paintPrompt(screen, model, title, lines, true, appName, version, visible);
        if (screen.hasTimeout()) screen.timeoutMs(-1);
        const int key = screen.getch();
        if (key == -1 || key == 27) {
            model.buffer.clear();
            model.cursor = 0;
            model.focus = "list";
            return "";
        }
        if (key == 10 || key == 13 || key == kKeyEnter) {
            const std::string text = model.buffer;
            model.buffer.clear();
            model.cursor = 0;
            return text;
        }
        if (key == 127 || key == 8 || key == kKeyBackspace) {
            model.backspace();
            continue;
        }
        if (key == kKeyLeft || key == kKeyRight || key == kKeyHome || key == kKeyEnd) {
            /* slide is private. Arrow keys in the box are ignored here except left/right via insert path. */
            continue;
        }
        if (key >= 32 && key < 127) model.insertChar(static_cast<char>(key));
    }
}

}  // namespace

Tui::Tui(std::string appNameIn, std::string versionIn, AboutPage *about, SelfManage *selfManage,
         std::string logFolder)
    : appName(std::move(appNameIn)),
      version(std::move(versionIn)),
      about_(about),
      selfManage_(selfManage),
      painter_(),
      systemLog_(std::move(logFolder)) {}

MenuPainter &Tui::painter() { return painter_; }
SystemLog &Tui::systemLog() { return systemLog_; }

std::string Tui::helloText() const { return "Welcome to " + appName + " " + version + "."; }

std::string Tui::helpOnScreen() const {
    return "help: usage for " + appName +
           ".\n"
           "Verbs: help, version, about, hello, self-install, version-check, self-update, self-uninstall.\n"
           "version shows the installed version and does not call pip.\n"
           "about shows the about page.\n"
           "hello prints a welcome line on the terminal and does not draw this menu.\n"
           "Menu row 1 hello shows that welcome line on this screen.\n"
           "This program does not join media or list files in this folder.\n"
           "version-check runs: python -m pip index versions " +
           appName +
           "\n"
           "self-update runs: python -m pip install --upgrade " +
           appName +
           "\n"
           "self-install runs: python -m pip install " +
           appName +
           "\n"
           "self-uninstall runs: python -m pip uninstall -y " +
           appName +
           "\n"
           "On the command line, self-uninstall also needs --force.\n"
           "Empty arguments on a terminal open this menu and do not run pip.";
}

std::string Tui::aboutText() const { return about_ == nullptr ? "" : about_->frameworkAbout(); }

std::vector<std::string> Tui::frontLines() const { return withPath(painter_, painter_.displayRows("front")); }
std::vector<std::string> Tui::selfLines() const { return withPath(painter_, painter_.displayRows("self")); }
std::vector<std::string> Tui::logLines() const { return withPath(painter_, painter_.displayRows("log")); }
std::vector<std::string> Tui::languageLines() const {
    return withPath(painter_, painter_.displayRows("lang"));
}

std::string Tui::onKind(const std::string &kind, MenuSession *session, Screen *screen) {
    if (kind == "help") return helpOnScreen();
    if (kind == "hello") return helloText();
    if (kind == "log-folder") return systemLog_.folderText();
    if ((kind == "version-check" || kind == "self-update" || kind == "self-install" ||
         kind == "self-uninstall") &&
        selfManage_ != nullptr) {
        int code = 0;
        return selfManage_->runPip(kind, &code);
    }
    if ((kind == "view-log" || kind == "clear-log") && screen != nullptr && session != nullptr) {
        const std::string folder = systemLog_.logDir();
        if (folder.empty()) return "No log folder.";
        const std::vector<std::string> files = systemLog_.logFiles();
        if (files.empty()) return "No log file in " + folder + ".";
        std::vector<std::string> lines = {"Log files:"};
        for (std::size_t index = 0; index < files.size(); ++index) {
            const auto slash = files[index].find_last_of('/');
            const std::string name = slash == std::string::npos ? files[index] : files[index].substr(slash + 1);
            lines.push_back(std::to_string(index + 1) + ". " + name);
        }
        lines.push_back("");
        lines.push_back(kind == "view-log" ? "Choose a log file:" : "Choose a log file to clear:");
        const std::string raw = readLine(painter_, session->model, *screen, lines, "system-log", appName, version);
        if (raw.empty() && session->model.focus == "list") return "";
        int picked = 0;
        try {
            picked = std::stoi(raw);
        } catch (...) {
            return "";
        }
        if (picked < 1 || picked > static_cast<int>(files.size())) return "";
        const std::string path = files[static_cast<std::size_t>(picked - 1)];
        const auto slash = path.find_last_of('/');
        const std::string name = slash == std::string::npos ? path : path.substr(slash + 1);
        if (kind == "clear-log") {
            const std::string answer = readLine(painter_, session->model, *screen,
                                                 {"Clear " + name + "? (y/n)"}, "system-log", appName, version);
            if (answer != "y" && answer != "yes" && answer != "Y" && answer != "YES") return "";
            if (!systemLog_.clearLog(path)) return "ERROR: " + name + " is not a log file in the log folder.";
            return "Cleared " + name + ".";
        }
        std::string body;
        if (!systemLog_.readLog(path, &body)) return "ERROR: " + name + " is not a log file in the log folder.";
        if (body.empty()) body = "(empty)";
        return name + "\n\n" + body;
    }
    return "";
}

int Tui::openTextMenu(bool stdoutIsTty) {
    if (!stdoutIsTty) return 1;
    const int code = runOnCurses([&](Screen &screen) {
        /* The kind handler needs the session. The pointer is set after construction. */
        std::unique_ptr<MenuSession> session;
        session = std::make_unique<MenuSession>(
            appName, version, &painter_,
            [&]() { return selfManage_ == nullptr ? appName + " " + version : selfManage_->localVersion(); },
            [&]() { return aboutText(); },
            [&](const std::string &kind) { return onKind(kind, session.get(), &screen); });
        return session->run(screen);
    });
    return code == 2 ? 1 : code;
}

int Tui::openAbout(bool stdoutIsTty) {
    if (!stdoutIsTty) {
        std::fputs(aboutText().c_str(), stdout);
        std::fputc('\n', stdout);
        return 0;
    }
    return runOnCurses([&](Screen &screen) {
        MenuSession session(appName, version, &painter_, []() { return std::string(); },
                            [&]() { return aboutText(); }, [](const std::string &) { return std::string(); });
        session.model.showResult(aboutText());
        painter_.paint(screen, session.model, appName, version);
        if (screen.hasTimeout()) screen.timeoutMs(-1);
        screen.getch();
        session.leaveToFront();
        return session.run(screen);
    });
}

}  // namespace hallo
