#include "utils.h"
#include "logger.h"
#include <coreinit/time.h>
#include <cstdio>
#include <optional>
#include <random>
#include <stdexcept>
#include <string>
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

namespace {
    std::optional<std::minstd_rand> sRandomEngine;
} // namespace

std::size_t GetRandomIndex(std::size_t size) {
    if (!sRandomEngine) {
        auto t = static_cast<std::uint64_t>(OSGetTime());
        std::seed_seq seeder{static_cast<std::uint32_t>(t),
                             static_cast<std::uint32_t>(t >> 32)};
        sRandomEngine.emplace(seeder);
    }
    std::uniform_int_distribution<std::size_t> dist{0, size - 1};
    return dist(*sRandomEngine);
}

std::filesystem::path ToLower(const std::filesystem::path &p) {
    std::string result;
    for (auto c : p.string()) {
        result.push_back(std::tolower(static_cast<unsigned char>(c)));
    }
    return result;
}
