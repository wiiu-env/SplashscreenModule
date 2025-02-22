#include "JPEGTexture.h"
#include <gx2/mem.h>
#include <cstdlib>
#include <cstring>
#include <turbojpeg.h>

GX2Texture *JPEG_LoadTexture(std::span<uint8_t> data) {
    GX2Texture *texture = nullptr;

    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return nullptr;
    }

    int height;
    int width;
    int subsamp;
    int colorspace;
    if (tjDecompressHeader3(handle,
                            data.data(), data.size(),
                            &width, &height,
                            &subsamp, &colorspace)) {
        goto error;
    }

    texture = static_cast<GX2Texture*>(std::malloc(sizeof(GX2Texture)));
    if (!texture) {
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
        goto error;
    }

    texture->surface.image = std::aligned_alloc(texture->surface.alignment,
                                                texture->surface.imageSize);
    if (!texture->surface.image) {
        goto error;
    }

    if (tjDecompress2(handle,
                      data.data(), data.size(),
                      static_cast<unsigned char*>(texture->surface.image),
                      width,
                      texture->surface.pitch * 4,
                      height,
                      TJPF_RGBA,
                      0)) {
        goto error;
    }

    tjDestroy(handle);

    GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_TEXTURE,
                  texture->surface.image, texture->surface.imageSize);
    return texture;

 error:
    if (texture) {
        std::free(texture->surface.image);
    }
    std::free(texture);
    tjDestroy(handle);
    return nullptr;
}
