#include "PNGTexture.h"
#include <cstring>
#include <gx2/mem.h>
#include <malloc.h>
#include <png.h>

static void png_read_data(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead) {
    void **data = (void **) png_get_io_ptr(png_ptr);

    memcpy(outBytes, *data, byteCountToRead);
    *((uint8_t **) data) += byteCountToRead;
}

GX2Texture *PNG_LoadTexture(std::span<uint8_t> data) {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return nullptr;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return nullptr;
    }

    png_set_read_fn(png_ptr, (void *) &data, png_read_data);

    png_read_info(png_ptr, info_ptr);

    uint32_t width  = 0;
    uint32_t height = 0;
    int bitDepth    = 0;
    int colorType   = -1;
    uint32_t retval = png_get_IHDR(png_ptr, info_ptr, &width, &height, &bitDepth, &colorType, nullptr, nullptr, nullptr);
    if (retval != 1) {
        return nullptr;
    }

    uint32_t bytesPerRow = png_get_rowbytes(png_ptr, info_ptr);
    auto *rowData        = new uint8_t[bytesPerRow];

    auto *texture = (GX2Texture *) malloc(sizeof(GX2Texture));
    *texture      = {};

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
        return nullptr;
    }

    texture->surface.image = memalign(texture->surface.alignment, texture->surface.imageSize);
    if (!texture->surface.image) {
        return nullptr;
    }

    for (uint32_t y = 0; y < height; y++) {
        uint32_t *out_data = (uint32_t *) texture->surface.image + (y * texture->surface.pitch);
        png_read_row(png_ptr, (png_bytep) rowData, nullptr);
        for (uint32_t x = 0; x < width; x++) {
            if (colorType == PNG_COLOR_TYPE_RGB_ALPHA) {
                uint32_t i = (x) * 4;
                *out_data  = rowData[i] << 24 | rowData[i + 1] << 16 | rowData[i + 2] << 8 | rowData[i + 3];
            } else if (colorType == PNG_COLOR_TYPE_RGB) {
                uint32_t i = (x) * 3;
                *out_data  = rowData[i] << 24 | rowData[i + 1] << 16 | rowData[i + 2] << 8 | 0xFF;
            }
            out_data++;
        }
    }

    delete[] rowData;
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    // todo: create texture with optimal tile format and use GX2CopySurface to convert from linear to tiled format
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_TEXTURE, texture->surface.image, texture->surface.imageSize);

    return texture;
}