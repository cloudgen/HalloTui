#include "system_log.h"

#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

namespace hallo {
namespace {

std::string realOf(const std::string &path) {
    char buffer[4096];
    if (::realpath(path.c_str(), buffer) == nullptr) return "";
    return buffer;
}

}  // namespace

SystemLog::SystemLog(std::string folder) : folder_(std::move(folder)) {}

const std::string &SystemLog::logDir() const { return folder_; }

bool SystemLog::owned(const std::string &path, std::string *resolved) const {
    if (folder_.empty()) return false;
    const std::string root = realOf(folder_);
    const std::string candidate = realOf(path);
    if (root.empty() || candidate.empty()) return false;
    const auto slash = candidate.find_last_of('/');
    if (slash == std::string::npos) return false;
    if (candidate.substr(0, slash) != root) return false;
    if (candidate.size() < 4 || candidate.substr(candidate.size() - 4) != ".log") return false;
    struct stat info;
    if (::stat(candidate.c_str(), &info) != 0 || !S_ISREG(info.st_mode)) return false;
    *resolved = candidate;
    return true;
}

std::vector<std::string> SystemLog::logFiles() const {
    std::vector<std::string> found;
    if (folder_.empty()) return found;
    DIR *dir = ::opendir(folder_.c_str());
    if (dir == nullptr) return found;
    while (dirent *entry = ::readdir(dir)) {
        const std::string name = entry->d_name;
        if (name == "." || name == "..") continue;
        std::string resolved;
        if (owned(folder_ + "/" + name, &resolved)) found.push_back(resolved);
    }
    ::closedir(dir);
    std::sort(found.begin(), found.end());
    return found;
}

bool SystemLog::readLog(const std::string &path, std::string *body) const {
    std::string resolved;
    if (!owned(path, &resolved)) return false;
    std::ifstream input(resolved);
    if (!input) return false;
    body->assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
    return true;
}

bool SystemLog::clearLog(const std::string &path) const {
    std::string resolved;
    if (!owned(path, &resolved)) return false;
    std::ofstream output(resolved, std::ios::trunc);
    return static_cast<bool>(output);
}

std::string SystemLog::folderText() const {
    if (folder_.empty()) return "No log folder.";
    return "Log folder: " + folder_;
}

}  // namespace hallo
