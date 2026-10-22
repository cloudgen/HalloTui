#ifndef HALLO_TEXT_WIDTH_H
#define HALLO_TEXT_WIDTH_H

#include <string>

namespace hallo {

/* Display columns. Wide and Fullwidth count as two. Ambiguous stays one. */
int displayWidth(const std::string &text);
std::string clipColumns(const std::string &text, int columns);
std::string clipColumnsRight(const std::string &text, int columns);

}  // namespace hallo

#endif
