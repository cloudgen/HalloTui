#ifndef HALLO_ABOUT_PAGE_H
#define HALLO_ABOUT_PAGE_H

#include <string>
#include <vector>

namespace hallo {

class AboutPage {
public:
    AboutPage(std::string appName, std::string version, std::string consoleName,
              std::string author, std::string updated, std::string homepage, std::string usage);

    std::string installKind(const std::string &path) const;
    std::string installSentence(const std::string &kind) const;
    std::vector<std::string> aboutBoxLines(const std::string &usage, const std::string &location,
                                           const std::string &kind) const;
    std::string frameworkAbout() const;
    std::string selfLocation() const;

private:
    bool sourceCheckout(const std::string &path) const;
    std::string appName_;
    std::string version_;
    std::string consoleName_;
    std::string author_;
    std::string updated_;
    std::string homepage_;
    std::string usage_;
};

}  // namespace hallo

#endif
