#ifndef HALLO_SELF_MANAGE_H
#define HALLO_SELF_MANAGE_H

#include <string>
#include <vector>

namespace hallo {

class SelfManage {
public:
    static constexpr const char *kDistribution = "HalloTui";
    SelfManage(std::string appName, std::string version);
    std::string localVersion() const;
    std::vector<std::string> argvFor(const std::string &verb) const;
    int emit(const std::string &verb) const;
    std::string runPip(const std::string &verb, int *code) const;

private:
    std::string appName_;
    std::string version_;
};

}  // namespace hallo

#endif
