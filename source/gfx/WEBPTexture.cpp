#include "WEBPTexture.h"
#include "utils/logger.h"
#include <cstdlib>
#include <cstring>
#include <gx2/mem.h>
#include <webp/decode.h>

GX2Texture *WEBP_LoadTexture(std::span<uint8_t> data) {
    GX2Texture *texture = nullptr;
    int width, height;

    if (!WebPGetInfo(data.data(), data.size(), &width, &height)) {
        DEBUG_FUNCTION_LINE_ERR("Failed to parse WEBP header\n");
        goto error;
    }

    texture = static_cast<GX2Texture *>(std::malloc(sizeof(GX2Texture)));
    if (!texture) {
        DEBUG_FUNCTION_LINE_ERR("Failed to allocate texture\n");
        goto error;
    }

    std::memset(texture, 0, sizeof(GX2Texture));
    texture->surface.width     = width;
    texture->surface.height    = height;
    texture->surface.depth     = 1;
    texture->surface.mipLevels = 1;
    texture->surface.format    = GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8;
    texture->surface.aa        = GX2_AA_MODE1X;
    texture->surface.use       = GX2_SURFACE_USE_TEXTURE;
    texture->surface.dim       = GX2_SURFACE_DIM_TEXTURE_2D;
    texture->surface.tileMode  = GX2_TILE_MODE_LINEAR_ALIGNED;
    texture->surface.swizzle   = 0;
    texture->viewFirstMip      = 0;
    texture->viewNumMips       = 1;
    texture->viewFirstSlice    = 0;
    texture->viewNumSlices     = 1;
    texture->compMap           = 0x0010203;
    GX2CalcSurfaceSizeAndAlignment(&texture->surface);
    GX2InitTextureRegs(texture);

    if (texture->surface.imageSize == 0) {
        DEBUG_FUNCTION_LINE_ERR("Texture is empty\n");
        goto error;
    }

    texture->surface.image = std::aligned_alloc(texture->surface.alignment,
                                                texture->surface.imageSize);
    if (!texture->surface.image) {
        DEBUG_FUNCTION_LINE_ERR("Failed to allocate surface for texture\n");
        goto error;
    }

    if (!WebPDecodeRGBAInto(data.data(), data.size(),
                            reinterpret_cast<uint8_t *>(texture->surface.image),
                            texture->surface.imageSize,
                            texture->surface.pitch * 4)) {
        DEBUG_FUNCTION_LINE_ERR("Failed to decode WEBP image\n");
        goto error;
    }

    GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_TEXTURE,
                  texture->surface.image, texture->surface.imageSize);

    return texture;

error:
    if (texture) {
        std::free(texture->surface.image);
    }
    std::free(texture);
    return nullptr;
}
