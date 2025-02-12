#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

bool LoadFileIntoBuffer(const std::filesystem::path &filename, std::vector<uint8_t> &buffer);
