#pragma once

#include <cstdint>
#include <gx2/texture.h>
#include <span>

GX2Texture *JPEG_LoadTexture(std::span<uint8_t> data);
