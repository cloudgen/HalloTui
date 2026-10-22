#ifndef HALLO_MENU_LANGUAGE_H
#define HALLO_MENU_LANGUAGE_H

#include <string>
#include <utility>
#include <vector>

namespace hallo {

struct LanguageCode {
    const char *code;
    const char *endonym;
    int number;
};

class MenuLanguage {
public:
    static const LanguageCode kCodes[13];
    static constexpr int kBlockFirst = 40;
    static constexpr int kBlockLast = 59;
    static constexpr const char *kAppDir = "HalloTui";
    static constexpr const char *kLeafName = "language";

    /* No argument reads HOME. An empty string stays empty and cannot save. */
    MenuLanguage();
    explicit MenuLanguage(std::string home);

    std::vector<std::string> codes() const;
    std::string endonym(const std::string &code) const;
    int numberFor(const std::string &code) const;
    std::string codeForNumber(int number) const;
    std::string leafPath() const;
    const std::string &code() const;
    void load();
    std::string save(const std::string &code);
    std::string text(const std::string &key) const;
    std::string pathLabel() const;
    std::string langLong(const std::string &endonym) const;
    std::string rowShort(const std::string &kind, const std::string &english) const;
    std::string rowLong(const std::string &kind, const std::string &english) const;
    std::string tokenKind(const std::string &token, const std::string &layer) const;

private:
    std::string readLeaf() const;
    std::string home_;
    std::string code_;
};

}  // namespace hallo

#endif
