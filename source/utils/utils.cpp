#include "logger.h"
#include <cstdint>
#include <cstdio>
#include <string_view>
#include <sys/stat.h>
#include <vector>

bool LoadFileIntoBuffer(std::string_view path, std::vector<uint8_t> &buffer) {
    struct stat st {};
    if (stat(path.data(), &st) < 0 || !S_ISREG(st.st_mode)) {
        DEBUG_FUNCTION_LINE_INFO("\"%s\" doesn't exists", path.data());
        return false;
    }

    FILE *f = fopen(path.data(), "rb");
    if (!f) {
        return false;
    }
    buffer.resize(st.st_size);

    if (fread(buffer.data(), 1, st.st_size, f) != st.st_size) {
        DEBUG_FUNCTION_LINE_WARN("Failed load %s", path.data());
        return false;
    }
    fclose(f);
    return true;
}