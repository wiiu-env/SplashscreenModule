#include "utils.h"
#include "logger.h"
#include <cstdio>
#include <stdexcept>
#include <sys/stat.h>
#include <vector>

bool LoadFileIntoBuffer(const std::filesystem::path &filename, std::vector<uint8_t> &buffer) {
    struct stat st {};
    if (stat(filename.c_str(), &st) < 0 || !S_ISREG(st.st_mode)) {
        DEBUG_FUNCTION_LINE_INFO("\"%s\" doesn't exists", filename.c_str());
        return false;
    }

    FILE *f = fopen(filename.c_str(), "rb");
    if (!f) {
        return false;
    }
    try {
        buffer.resize(st.st_size);
    } catch (std::bad_alloc &e) {
        DEBUG_FUNCTION_LINE_WARN("Failed allocate memory for %s: %s", filename.c_str(), e.what());
    }

    if (fread(buffer.data(), 1, st.st_size, f) != st.st_size) {
        DEBUG_FUNCTION_LINE_WARN("Failed load %s", filename.c_str());
        fclose(f);
        return false;
    }
    fclose(f);
    return true;
}
