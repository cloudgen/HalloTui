#include "text_width.h"

#include <cstdint>
#include <vector>

namespace hallo {
namespace {

struct Cp {
    char32_t code;
    int bytes;
};

bool wideCode(char32_t code) {
    /* Unicode East Asian Width W and F. Ambiguous ranges are absent on purpose. */
    if (code >= 0x1100 && code <= 0x115F) return true;
    if (code == 0x2329 || code == 0x232A) return true;
    if (code >= 0x2E80 && code <= 0xA4CF) return true;
    if (code >= 0xAC00 && code <= 0xD7A3) return true;
    if (code >= 0xF900 && code <= 0xFAFF) return true;
    if (code >= 0xFE10 && code <= 0xFE19) return true;
    if (code >= 0xFE30 && code <= 0xFE6F) return true;
    if (code >= 0xFF00 && code <= 0xFF60) return true;
    if (code >= 0xFFE0 && code <= 0xFFE6) return true;
    if (code >= 0x1F300 && code <= 0x1F64F) return true;
    if (code >= 0x1F900 && code <= 0x1F9FF) return true;
    if (code >= 0x20000 && code <= 0x3FFFD) return true;
    return false;
}

int widthOf(char32_t code) {
    return wideCode(code) ? 2 : 1;
}

std::vector<Cp> decode(const std::string &text) {
    std::vector<Cp> out;
    const auto *bytes = reinterpret_cast<const unsigned char *>(text.data());
    std::size_t i = 0;
    while (i < text.size()) {
        unsigned char lead = bytes[i];
        char32_t code = lead;
        int need = 1;
        if ((lead & 0x80) == 0) {
            need = 1;
            code = lead;
        } else if ((lead & 0xE0) == 0xC0 && i + 1 < text.size()) {
            need = 2;
            code = ((lead & 0x1F) << 6) | (bytes[i + 1] & 0x3F);
        } else if ((lead & 0xF0) == 0xE0 && i + 2 < text.size()) {
            need = 3;
            code = ((lead & 0x0F) << 12) | ((bytes[i + 1] & 0x3F) << 6) | (bytes[i + 2] & 0x3F);
        } else if ((lead & 0xF8) == 0xF0 && i + 3 < text.size()) {
            need = 4;
            code = ((lead & 0x07) << 18) | ((bytes[i + 1] & 0x3F) << 12) |
                   ((bytes[i + 2] & 0x3F) << 6) | (bytes[i + 3] & 0x3F);
        } else {
            need = 1;
            code = lead;
        }
        out.push_back(Cp{code, need});
        i += static_cast<std::size_t>(need);
    }
    return out;
}

std::string encodeOne(char32_t code) {
    std::string out;
    if (code <= 0x7F) {
        out.push_back(static_cast<char>(code));
    } else if (code <= 0x7FF) {
        out.push_back(static_cast<char>(0xC0 | (code >> 6)));
        out.push_back(static_cast<char>(0x80 | (code & 0x3F)));
    } else if (code <= 0xFFFF) {
        out.push_back(static_cast<char>(0xE0 | (code >> 12)));
        out.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (code & 0x3F)));
    } else {
        out.push_back(static_cast<char>(0xF0 | (code >> 18)));
        out.push_back(static_cast<char>(0x80 | ((code >> 12) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (code & 0x3F)));
    }
    return out;
}

}  // namespace

int displayWidth(const std::string &text) {
    int total = 0;
    for (const Cp &item : decode(text)) {
        total += widthOf(item.code);
    }
    return total;
}

std::string clipColumns(const std::string &text, int columns) {
    if (columns <= 0) return "";
    std::string kept;
    int used = 0;
    for (const Cp &item : decode(text)) {
        int width = widthOf(item.code);
        if (used + width > columns) break;
        kept += encodeOne(item.code);
        used += width;
    }
    return kept;
}

std::string clipColumnsRight(const std::string &text, int columns) {
    if (columns <= 0) return "";
    const std::vector<Cp> codes = decode(text);
    int used = 0;
    int start = static_cast<int>(codes.size());
    for (int index = static_cast<int>(codes.size()) - 1; index >= 0; --index) {
        int width = widthOf(codes[static_cast<std::size_t>(index)].code);
        if (used + width > columns) break;
        used += width;
        start = index;
    }
    std::string kept;
    for (int index = start; index < static_cast<int>(codes.size()); ++index) {
        kept += encodeOne(codes[static_cast<std::size_t>(index)].code);
    }
    return kept;
}

}  // namespace hallo
