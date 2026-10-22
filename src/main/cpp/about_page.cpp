#include "about_page.h"

#include "project.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <unistd.h>

namespace hallo {
namespace {

std::string homeDir() {
    const char *home = std::getenv("HOME");
    return home == nullptr ? std::string() : std::string(home);
}

std::string stampNow() {
    const std::time_t now = std::time(nullptr);
    std::tm local {};
    localtime_r(&now, &local);
    char buffer[64];
    std::strftime(buffer, sizeof buffer, "%Y-%m-%d %H:%M:%S", &local);
    return std::string(buffer) + ".000000";
}

bool isDir(const std::string &path) {
    struct stat info;
    return ::stat(path.c_str(), &info) == 0 && S_ISDIR(info.st_mode);
}

}  // namespace

AboutPage::AboutPage(std::string appName, std::string version, std::string consoleName,
                     std::string author, std::string updated, std::string homepage, std::string usage)
    : appName_(std::move(appName)),
      version_(std::move(version)),
      consoleName_(std::move(consoleName)),
      author_(std::move(author)),
      updated_(std::move(updated)),
      homepage_(std::move(homepage)),
      usage_(std::move(usage)) {}

bool AboutPage::sourceCheckout(const std::string &path) const {
    /* A binary under a checkout is uninstalled even when that checkout is in $HOME.
       The executable path itself does not contain src/main/cpp. */
    std::string cursor = path;
    for (int hop = 0; hop < 8; ++hop) {
        const auto slash = cursor.find_last_of('/');
        if (slash == std::string::npos || slash == 0) return false;
        cursor = cursor.substr(0, slash);
        const bool buildFile =
            std::ifstream(cursor + "/Makefile").good() || std::ifstream(cursor + "/project.ini").good();
        if (buildFile && isDir(cursor + "/src/main/cpp")) return true;
    }
    return false;
}

std::string AboutPage::selfLocation() const {
    char buffer[4096];
    const ssize_t count = ::readlink("/proc/self/exe", buffer, sizeof buffer - 1);
    if (count > 0) {
        buffer[count] = '\0';
        return buffer;
    }
    return consoleName_;
}

std::string AboutPage::installKind(const std::string &path) const {
    const bool inHome = !homeDir().empty() && (path == homeDir() || path.compare(0, homeDir().size() + 1, homeDir() + "/") == 0);
    if (sourceCheckout(path)) return "uninstalled";
    if (inHome) return "local";
    if (path.compare(0, 5, "/usr/") == 0 || path.compare(0, 5, "/opt/") == 0 ||
        path.compare(0, 5, "/bin/") == 0) {
        return "global";
    }
    return "uninstalled";
}

std::string AboutPage::installSentence(const std::string &kind) const {
    if (kind == "global") return "You are using the GLOBAL INSTALLED version, location:";
    if (kind == "local") return "You are using the LOCAL INSTALLED version, location:";
    return "You are using an UNINSTALLED version, location:";
}

std::vector<std::string> AboutPage::aboutBoxLines(const std::string &usage, const std::string &location,
                                                 const std::string &kind) const {
    const std::vector<std::string> msg = {
        appName_ + " (" + version_ + ") by " + author_ + " on " + updated_,
        "",
        installSentence(kind),
        "    " + location,
        "",
        "Basic Usage:",
        "    " + usage,
        "",
        "Please visit our homepage: ",
        "    \"" + homepage_ + "\"",
    };
    std::size_t maxLen = 0;
    for (const std::string &line : msg) maxLen = std::max(maxLen, line.size());
    const std::string star(maxLen + 4, '*');
    const std::string blank(maxLen, ' ');
    std::vector<std::string> lines;
    lines.push_back(star);
    lines.push_back("* " + blank + " *");
    for (const std::string &line : msg) {
        lines.push_back("* " + line + std::string(maxLen - line.size(), ' ') + " *");
    }
    lines.push_back("* " + blank + " *");
    lines.push_back(star);
    return lines;
}

std::string AboutPage::frameworkAbout() const {
    const std::string location = selfLocation();
    const std::string kind = installKind(location);
    utsname host {};
    ::uname(&host);
    const std::string user = std::getenv("USER") == nullptr ? "" : std::getenv("USER");
    const std::string shell = std::getenv("SHELL") == nullptr ? "" : std::getenv("SHELL");
    const std::string pid = std::to_string(::getpid());
    const std::string leaf = user.empty() ? std::string("cache-") + appName_ + "-" + pid
                                          : std::string("cache-") + appName_ + "-" + user + "-" + pid;
    const std::string preferred = "/dev/shm/cache/" + leaf;
    const std::string first = "/tmp/cache/" + leaf;
    const std::string second = homeDir().empty() ? "" : homeDir() + "/.cache/cache-" + appName_ + "-" + pid;
    std::string used = second;
    if (isDir("/dev/shm")) used = preferred;
    else if (isDir("/tmp")) used = first;
    const std::string persistence = homeDir().empty() ? "" : homeDir() + "/.local/" + appName_;
    std::ostringstream page;
    page << appName_ << " " << version_ << "\n"
         << "Domain: " << kDomainLine << "\n"
         << "Runtime tools: none\n"
         << "Entry points: " << consoleName_ << "\n"
         << "\n"
         << stampNow() << " " << appName_ << "(v" << version_ << ")  [CHECK SYSTEM]:\n"
         << "  Now checking your operation system!\n"
         << "    Compiler: g++ " << __VERSION__ << "\n"
         << "    C++ standard: C++17\n"
         << "    Operation System: " << host.sysname << " " << host.release << "\n"
         << "    Architecture: " << host.machine << "\n"
         << "    Current User: " << user << "\n"
         << "    Shell: " << shell << "\n"
         << "    Inside docker container: " << (std::ifstream("/.dockerenv").good() ? "True" : "False") << "\n"
         << "    Location: " << location << "\n"
         << "    PID: " << pid << "\n"
         << "    Cache folder used: " << used << "\n"
         << "    Cache folder (preferred): " << preferred << "\n"
         << "    Cache folder (1st fallback): " << first << "\n"
         << "    Cache folder (2nd fallback): " << second << "\n"
         << "    Persistence storage: " << persistence << "\n"
         << "    TTY / Interactive: " << (::isatty(STDOUT_FILENO) ? "yes" : "no") << "\n"
         << "\n";
    for (const std::string &line : aboutBoxLines(usage_, location, kind)) page << line << "\n";
    std::string text = page.str();
    if (!text.empty() && text.back() == '\n') text.pop_back();
    return text;
}

}  // namespace hallo
