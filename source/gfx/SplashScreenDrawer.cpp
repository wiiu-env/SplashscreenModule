#include "SplashScreenDrawer.h"
#include "JPEGTexture.h"
#include "PNGTexture.h"
#include "ShaderSerializer.h"
#include "TGATexture.h"
#include "gfx.h"
#include "utils/logger.h"
#include "utils/utils.h"
#include <cctype>
#include <coreinit/time.h>
#include <cstdlib>
#include <gx2/draw.h>
#include <gx2/mem.h>
#include <gx2r/draw.h>
#include <random>
#include <whb/log.h>

/*
constexpr const char *s_textureVertexShader = R"(
#version 450

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

layout(location = 0) out vec2 TexCoord;

void main()
{
    TexCoord = aTexCoord;
    gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
}
)";
 */

constexpr uint8_t s_textureVertexShaderCompiled[] = {
        0x00, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0x8A, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
        0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
        0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
        0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
        0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
        0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
        0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
        0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
        0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
        0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
        0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E,
        0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x80, 0x89, 0x00, 0x40, 0x01, 0xC0, 0xC8, 0x0F, 0x00, 0x94,
        0x3C, 0xA0, 0x00, 0xC0, 0x08, 0x0B, 0x00, 0x94, 0x05, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x80,
        0x00, 0x00, 0x00, 0x80, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0F, 0xFF,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x61, 0x50, 0x6F, 0x73,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x61, 0x54, 0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72,
        0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00};

/*
constexpr const char *s_texturePixelShader = R"(
#version 450
#extension GL_ARB_shading_language_420pack: enable

layout(location = 0) in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

layout(binding = 0) uniform sampler2D inTexture;

void main()
{
    FragColor = texture(inTexture, TexCoord);
}
)";*/
constexpr uint8_t s_texturePixelShaderCompiled[] = {
        0x00, 0x20, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x10, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x8A,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20,
        0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0xC0,
        0x88, 0x06, 0x20, 0x94, 0x10, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0D, 0xF0,
        0x00, 0x00, 0x80, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0F, 0xFF,
        0x00, 0x00, 0x00, 0x01, 0x69, 0x6E, 0x54, 0x65, 0x78, 0x74, 0x75, 0x72,
        0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/** empty png as fallback */
uint8_t empty_png[119] = {
        0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D,
        0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02,
        0x08, 0x02, 0x00, 0x00, 0x00, 0xFD, 0xD4, 0x9A, 0x73, 0x00, 0x00, 0x00,
        0x01, 0x73, 0x52, 0x47, 0x42, 0x00, 0xAE, 0xCE, 0x1C, 0xE9, 0x00, 0x00,
        0x00, 0x04, 0x67, 0x41, 0x4D, 0x41, 0x00, 0x00, 0xB1, 0x8F, 0x0B, 0xFC,
        0x61, 0x05, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00, 0x00,
        0x16, 0x25, 0x00, 0x00, 0x16, 0x25, 0x01, 0x49, 0x52, 0x24, 0xF0, 0x00,
        0x00, 0x00, 0x0C, 0x49, 0x44, 0x41, 0x54, 0x18, 0x57, 0x63, 0x40, 0x02,
        0x0C, 0x0C, 0x00, 0x00, 0x0E, 0x00, 0x01, 0x7A, 0xB1, 0xB9, 0x30, 0x00,
        0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82};

static std::filesystem::path ToLower(const std::filesystem::path &p) {
    std::string result;
    for (auto c : p.string()) {
        result.push_back(std::tolower(static_cast<unsigned char>(c)));
    }
    return result;
}

static GX2Texture *LoadImageAsTexture(const std::filesystem::path &filename) {
    std::vector<uint8_t> buffer;
    if (LoadFileIntoBuffer(filename, buffer)) {
        auto ext = ToLower(filename.extension());
        if (ext == ".png") {
            return PNG_LoadTexture(buffer);
        } else if (ext == ".jpg" || ext == ".jpeg") {
            return JPEG_LoadTexture(buffer);
        } else if (ext == ".tga") {
            return TGA_LoadTexture(buffer);
        }
    }
    return nullptr;
}

SplashScreenDrawer::SplashScreenDrawer(const std::filesystem::path &splash_base_path) {
    mTexture = LoadImageAsTexture(splash_base_path / "splash.png");
    if (!mTexture) {
        mTexture = LoadImageAsTexture(splash_base_path / "splash.jpg");
    }
    if (!mTexture) {
        mTexture = LoadImageAsTexture(splash_base_path / "splash.jpeg");
    }
    if (!mTexture) {
        mTexture = LoadImageAsTexture(splash_base_path / "splash.tga");
    }
    if (!mTexture) {
        // try to load a random one from "splashes/*"
        try {
            std::vector<std::filesystem::path> candidates;
            for (const auto &entry : std::filesystem::directory_iterator{splash_base_path / "splashes"}) {
                if (!entry.is_regular_file()) {
                    continue;
                }
                auto ext = ToLower(entry.path().extension());
                if (ext == ".png" || ext == ".tga" || ext == ".jpg" || ext == ".jpeg") {
                    candidates.push_back(entry.path());
                }
            }
            if (!candidates.empty()) {
                auto t = static_cast<std::uint64_t>(OSGetTime());
                std::seed_seq seed{static_cast<std::uint32_t>(t),
                                   static_cast<std::uint32_t>(t >> 32)};
                std::minstd_rand eng{seed};
                std::uniform_int_distribution<std::size_t> dist{0, candidates.size() - 1};
                auto selected = dist(eng);
                mTexture      = LoadImageAsTexture(candidates[selected]);
            }
        } catch (std::exception &) {}
    }
    if (!mTexture) {
        mTexture = PNG_LoadTexture(empty_png);
    }
    if (!mTexture) {
        return;
    }

    // create shader group
    mVertexShaderWrapper = DeserializeVertexShader(s_textureVertexShaderCompiled);
    mPixelShaderWrapper  = DeserializePixelShader(s_texturePixelShaderCompiled);

    mShaderGroup              = {};
    mShaderGroup.vertexShader = mVertexShaderWrapper->getVertexShader();
    mShaderGroup.pixelShader  = mPixelShaderWrapper->getPixelShader();

    GX2Invalidate(GX2_INVALIDATE_MODE_CPU_SHADER, mShaderGroup.vertexShader->program, mShaderGroup.vertexShader->size);
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU_SHADER, mShaderGroup.pixelShader->program, mShaderGroup.pixelShader->size);

    GX2SetShaderMode(GX2_SHADER_MODE_UNIFORM_BLOCK);

    GfxInitShaderAttribute(&mShaderGroup, "aPos", 0, 0, GX2_ATTRIB_FORMAT_FLOAT_32_32);
    GfxInitShaderAttribute(&mShaderGroup, "aTexCoord", 1, 0, GX2_ATTRIB_FORMAT_FLOAT_32_32);
    GfxInitFetchShader(&mShaderGroup);

    // upload vertex position
    mPositionBuffer.flags     = GX2R_RESOURCE_BIND_VERTEX_BUFFER | GX2R_RESOURCE_USAGE_CPU_READ | GX2R_RESOURCE_USAGE_CPU_WRITE | GX2R_RESOURCE_USAGE_GPU_READ;
    mPositionBuffer.elemSize  = 2 * sizeof(float);
    mPositionBuffer.elemCount = 4;
    GX2RCreateBuffer(&mPositionBuffer);
    void *posUploadBuffer = GX2RLockBufferEx(&mPositionBuffer, GX2R_RESOURCE_BIND_NONE);
    memcpy(posUploadBuffer, mPositionData, mPositionBuffer.elemSize * mPositionBuffer.elemCount);
    GX2RUnlockBufferEx(&mPositionBuffer, GX2R_RESOURCE_BIND_NONE);

    // upload texture coords
    mTexCoordBuffer.flags     = GX2R_RESOURCE_BIND_VERTEX_BUFFER | GX2R_RESOURCE_USAGE_CPU_READ | GX2R_RESOURCE_USAGE_CPU_WRITE | GX2R_RESOURCE_USAGE_GPU_READ;
    mTexCoordBuffer.elemSize  = 2 * sizeof(float);
    mTexCoordBuffer.elemCount = 4;
    GX2RCreateBuffer(&mTexCoordBuffer);
    void *coordsUploadBuffer = GX2RLockBufferEx(&mTexCoordBuffer, GX2R_RESOURCE_BIND_NONE);
    memcpy(coordsUploadBuffer, mTexCoords, mTexCoordBuffer.elemSize * mTexCoordBuffer.elemCount);
    GX2RUnlockBufferEx(&mTexCoordBuffer, GX2R_RESOURCE_BIND_NONE);

    GX2InitSampler(&mSampler, GX2_TEX_CLAMP_MODE_CLAMP, GX2_TEX_XY_FILTER_MODE_LINEAR);
}

void SplashScreenDrawer::Draw() {
    if (!mTexture) {
        DEBUG_FUNCTION_LINE_INFO("Texture is missing");
        return;
    }

    GfxBeginRender();

    GfxClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    GX2SetFetchShader(&mShaderGroup.fetchShader);
    GX2SetVertexShader(mShaderGroup.vertexShader);
    GX2SetPixelShader(mShaderGroup.pixelShader);
    GX2SetShaderMode(GX2_SHADER_MODE_UNIFORM_BLOCK);

    GX2RSetAttributeBuffer(&mPositionBuffer, 0, mPositionBuffer.elemSize, 0);
    GX2RSetAttributeBuffer(&mTexCoordBuffer, 1, mTexCoordBuffer.elemSize, 0);
    GX2SetPixelTexture(mTexture, mShaderGroup.pixelShader->samplerVars[0].location);
    GX2SetPixelSampler(&mSampler, mShaderGroup.pixelShader->samplerVars[0].location);

    GX2DrawEx(GX2_PRIMITIVE_MODE_QUADS, 4, 0, 1);

    GfxFinishRender();
}

SplashScreenDrawer::~SplashScreenDrawer() {
    GX2RDestroyBufferEx(&mPositionBuffer, GX2R_RESOURCE_BIND_NONE);
    GX2RDestroyBufferEx(&mTexCoordBuffer, GX2R_RESOURCE_BIND_NONE);
    if (mTexture) {
        if (mTexture->surface.image != nullptr) {
            std::free(mTexture->surface.image);
            mTexture->surface.image = nullptr;
        }
        std::free(mTexture);
        mTexture = nullptr;
    }
}
