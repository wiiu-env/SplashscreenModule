#include "JPEGTexture.h"
#include "utils/logger.h"
#include <cstdlib>
#include <cstring>
#include <gx2/mem.h>
#include <turbojpeg.h>

GX2Texture *JPEG_LoadTexture(std::span<uint8_t> data) {
    GX2Texture *texture = nullptr;
    int height;
    int width;

    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        goto error;
    }

    if (tj3DecompressHeader(handle, data.data(), data.size())) {
        DEBUG_FUNCTION_LINE_ERR("Failed to parse JPEG header: %s\n", tj3GetErrorStr(handle));
        goto error;
    }

    width  = tj3Get(handle, TJPARAM_JPEGWIDTH);
    height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
    if (width == -1 || height == -1) {
        DEBUG_FUNCTION_LINE_ERR("Unknown JPEG image size\n");
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

    if (tj3Decompress8(handle,
                       data.data(), data.size(),
                       static_cast<unsigned char *>(texture->surface.image),
                       texture->surface.pitch * 4,
                       TJPF_RGBA)) {
        DEBUG_FUNCTION_LINE_ERR("Failed to read JPEG image: %s\n", tj3GetErrorStr(handle));
        goto error;
    }

    tj3Destroy(handle);

    GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_TEXTURE,
                  texture->surface.image, texture->surface.imageSize);

    return texture;

error:
    if (texture) {
        std::free(texture->surface.image);
    }
    std::free(texture);
    tj3Destroy(handle);
    return nullptr;
}
