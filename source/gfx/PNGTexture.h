#pragma once

#include <gx2/texture.h>
#include <span>
#include <cstdint>

GX2Texture *PNG_LoadTexture(std::span<uint8_t> data);


