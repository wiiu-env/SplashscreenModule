#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <vector>

bool LoadFileIntoBuffer(const std::filesystem::path &filename, std::vector<uint8_t> &buffer);

std::size_t GetRandomIndex(std::size_t size);

std::filesystem::path ToLower(const std::filesystem::path &p);
