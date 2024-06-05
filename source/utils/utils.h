#pragma once

#include <cstdint>
#include <string_view>
#include <sys/stat.h>
#include <vector>

bool LoadFileIntoBuffer(std::string_view path, std::vector<uint8_t> &buffer);

static inline bool existsAsFile(std::string_view path) {
    struct stat st {};
    if (stat(path.data(), &st) >= 0 && S_ISREG(st.st_mode)) {
        DEBUG_FUNCTION_LINE_VERBOSE("\"%s\" exists", path.data());
        return true;
    } else {
        DEBUG_FUNCTION_LINE_VERBOSE("\"%s\" doesn't exists", path.data());
    }
    return false;
}