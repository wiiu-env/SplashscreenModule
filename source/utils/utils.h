#pragma once

#include <cstdint>
#include <string_view>
#include <sys/stat.h>
#include <vector>

bool LoadFileIntoBuffer(std::string_view path, std::vector<uint8_t> &buffer);
