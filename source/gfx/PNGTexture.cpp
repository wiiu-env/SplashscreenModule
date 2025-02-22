#include "PNGTexture.h"
#include "utils/logger.h"
#include <cstdlib>
#include <cstring>
#include <gx2/mem.h>
#include <png.h>

GX2Texture *PNG_LoadTexture(std::span<uint8_t> data) {
    GX2Texture *texture = nullptr;

    png_image image{};
    image.version = PNG_IMAGE_VERSION;

    if (!png_image_begin_read_from_memory(&image, data.data(), data.size())) {
        DEBUG_FUNCTION_LINE_ERR("Failed to parse PNG header: %s\n", image.message);
        goto error;
    }

    // Request the output to always be RGBA
    image.format = PNG_FORMAT_RGBA;

    texture = static_cast<GX2Texture *>(std::malloc(sizeof(GX2Texture)));
    if (!texture) {
        DEBUG_FUNCTION_LINE_ERR("Failed to allocate texture\n");
        goto error;
    }

    std::memset(texture, 0, sizeof(GX2Texture));
    texture->surface.width     = image.width;
    texture->surface.height    = image.height;
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

    if (!png_image_finish_read(&image, nullptr,
                               texture->surface.image,
                               texture->surface.pitch * 4,
                               nullptr)) {
        DEBUG_FUNCTION_LINE_ERR("Failed to read PNG image: %s\n", image.message);
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
    png_image_free(&image);
    return nullptr;
}
