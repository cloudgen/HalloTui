#ifndef HALLO_MENU_TEXT_DATA_H
#define HALLO_MENU_TEXT_DATA_H

#include <string>

namespace hallo {

std::string menuText(const std::string &code, const std::string &key);
std::string frontTokenKind(const std::string &token);
std::string langTokenCode(const std::string &token);
std::string shortKeyFor(const std::string &kind);
std::string longKeyFor(const std::string &kind);

}  // namespace hallo

#endif
