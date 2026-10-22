#include "self_manage.h"

#include <array>
#include <cstdio>
#include <sys/wait.h>
#include <unistd.h>

namespace hallo {

SelfManage::SelfManage(std::string appName, std::string version)
    : appName_(std::move(appName)), version_(std::move(version)) {}

std::string SelfManage::localVersion() const { return appName_ + " " + version_; }

std::vector<std::string> SelfManage::argvFor(const std::string &verb) const {
    const char *py = "python3";
    if (verb == "version-check") return {py, "-m", "pip", "index", "versions", kDistribution};
    if (verb == "self-update") return {py, "-m", "pip", "install", "--upgrade", kDistribution};
    if (verb == "self-install") return {py, "-m", "pip", "install", kDistribution};
    if (verb == "self-uninstall") return {py, "-m", "pip", "uninstall", "-y", kDistribution};
    return {};
}

std::string SelfManage::runPip(const std::string &verb, int *code) const {
    const std::vector<std::string> argv = argvFor(verb);
    std::string shown = "$";
    std::string command;
    for (const std::string &part : argv) {
        shown += " " + part;
        if (!command.empty()) command += " ";
        command += part;
    }
    command += " 2>&1";
    std::string output;
    FILE *pipe = ::popen(command.c_str(), "r");
    if (pipe == nullptr) {
        if (code) *code = 1;
        return shown + "\npip exited 1";
    }
    std::array<char, 512> buffer {};
    while (std::fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr) {
        output += buffer.data();
    }
    const int status = ::pclose(pipe);
    const int exitCode = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
    if (code) *code = exitCode;
    while (!output.empty() && (output.back() == '\n' || output.back() == '\r')) output.pop_back();
    std::string chunks = shown;
    if (verb == "version-check") chunks = localVersion() + "\n" + chunks;
    if (!output.empty()) chunks += "\n" + output;
    else if (exitCode != 0) chunks += "\npip exited " + std::to_string(exitCode);
    return chunks;
}

int SelfManage::emit(const std::string &verb) const {
    int code = 1;
    const std::string text = runPip(verb, &code);
    std::fputs(text.c_str(), stdout);
    std::fputc('\n', stdout);
    return code;
}

}  // namespace hallo
