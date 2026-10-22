#ifndef HALLO_SYSTEM_LOG_H
#define HALLO_SYSTEM_LOG_H

#include <string>
#include <vector>

namespace hallo {

class SystemLog {
public:
    explicit SystemLog(std::string folder = "");
    const std::string &logDir() const;
    std::vector<std::string> logFiles() const;
    /* Empty optional is a refused path. The string may be empty for an empty file. */
    bool readLog(const std::string &path, std::string *body) const;
    bool clearLog(const std::string &path) const;
    std::string folderText() const;

private:
    bool owned(const std::string &path, std::string *resolved) const;
    std::string folder_;
};

}  // namespace hallo

#endif
