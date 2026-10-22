#include "menu_language.h"

#include "menu_text_data.h"

#include <cerrno>
#include <cstdlib>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

namespace hallo {

const LanguageCode MenuLanguage::kCodes[13] = {
    {"en", "English", 41},
    {"zh-Hans", "简体中文", 42},
    {"zh-Hant", "繁體中文", 43},
    {"es", "Español", 44},
    {"ar", "العربية", 45},
    {"fr", "Français", 46},
    {"pt", "Português", 47},
    {"ru", "Русский", 48},
    {"de", "Deutsch", 49},
    {"ja", "日本語", 50},
    {"ko", "한국어", 51},
    {"nl", "Nederlands", 52},
    {"el", "Ελληνικά", 53},
};

namespace {

bool knownCode(const std::string &code) {
    for (const LanguageCode &item : MenuLanguage::kCodes) {
        if (code == item.code) return true;
    }
    return false;
}

std::string envOrEmpty(const char *name) {
    const char *value = std::getenv(name);
    return value == nullptr ? std::string() : std::string(value);
}

}  // namespace

MenuLanguage::MenuLanguage() : home_(envOrEmpty("HOME")), code_("en") { load(); }

MenuLanguage::MenuLanguage(std::string home) : home_(std::move(home)), code_("en") { load(); }

std::vector<std::string> MenuLanguage::codes() const {
    std::vector<std::string> out;
    for (const LanguageCode &item : kCodes) out.emplace_back(item.code);
    return out;
}

std::string MenuLanguage::endonym(const std::string &code) const {
    for (const LanguageCode &item : kCodes) {
        if (code == item.code) return item.endonym;
    }
    return "";
}

int MenuLanguage::numberFor(const std::string &code) const {
    for (const LanguageCode &item : kCodes) {
        if (code == item.code) return item.number;
    }
    return -1;
}

std::string MenuLanguage::codeForNumber(int number) const {
    for (const LanguageCode &item : kCodes) {
        if (item.number == number) return item.code;
    }
    return "";
}

std::string MenuLanguage::leafPath() const {
    if (home_.empty()) return "";
    return home_ + "/.local/" + kAppDir + "/" + kLeafName;
}

const std::string &MenuLanguage::code() const { return code_; }

void MenuLanguage::load() {
    const std::string forced = envOrEmpty("VIDEOJOIN_LANG");
    if (knownCode(forced)) {
        code_ = forced;
        return;
    }
    code_ = readLeaf();
}

std::string MenuLanguage::readLeaf() const {
    const std::string path = leafPath();
    if (path.empty()) return "en";
    std::ifstream input(path);
    if (!input) return "en";
    std::string line;
    std::getline(input, line);
    while (!line.empty() && (line.back() == '\r' || line.back() == ' ' || line.back() == '\t')) {
        line.pop_back();
    }
    std::size_t start = 0;
    while (start < line.size() && (line[start] == ' ' || line[start] == '\t')) ++start;
    line = line.substr(start);
    if (knownCode(line)) return line;
    return "en";
}

bool makeTree(const std::string &folder) {
    if (folder.empty() || folder == "/") return true;
    struct stat info;
    if (::stat(folder.c_str(), &info) == 0) return S_ISDIR(info.st_mode);
    const auto slash = folder.find_last_of('/');
    if (slash != std::string::npos && slash > 0) {
        if (!makeTree(folder.substr(0, slash))) return false;
    }
    if (::mkdir(folder.c_str(), 0700) != 0 && errno != EEXIST) return false;
    return true;
}

std::string MenuLanguage::save(const std::string &code) {
    if (!knownCode(code)) return text("save_failed");
    const std::string path = leafPath();
    if (path.empty()) return text("save_failed");
    const std::string previous = code_;
    const std::string folder = path.substr(0, path.find_last_of('/'));
    if (!makeTree(folder)) {
        code_ = previous;
        return text("save_failed");
    }
    ::chmod(folder.c_str(), 0700);
    const int descriptor = ::open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (descriptor < 0) {
        code_ = previous;
        return text("save_failed");
    }
    const std::string body = code + "\n";
    const ssize_t wrote = ::write(descriptor, body.data(), body.size());
    ::close(descriptor);
    if (wrote != static_cast<ssize_t>(body.size())) {
        code_ = previous;
        return text("save_failed");
    }
    ::chmod(path.c_str(), 0600);
    code_ = code;
    return text("saved");
}

std::string MenuLanguage::text(const std::string &key) const {
    return menuText(code_, key);
}

std::string MenuLanguage::pathLabel() const { return text("path"); }

std::string MenuLanguage::langLong(const std::string &name) const {
    std::string pattern = text("lang_long");
    const std::string token = "{name}";
    const std::size_t at = pattern.find(token);
    if (at == std::string::npos) return pattern;
    pattern.replace(at, token.size(), name);
    return pattern;
}

std::string MenuLanguage::rowShort(const std::string &kind, const std::string &english) const {
    const std::string key = shortKeyFor(kind);
    if (key.empty()) return english;
    return text(key);
}

std::string MenuLanguage::rowLong(const std::string &kind, const std::string &english) const {
    const std::string key = longKeyFor(kind);
    if (key.empty()) return english;
    return text(key);
}

std::string MenuLanguage::tokenKind(const std::string &token, const std::string &layer) const {
    if (layer == "lang") {
        const std::string code = langTokenCode(token);
        if (code.empty()) return "";
        return "set-" + code;
    }
    if (layer == "front") return frontTokenKind(token);
    return "";
}

}  // namespace hallo
