#include <simple-2d/graphics.h>
#include <simple-2d/utils.h>
#include <SDL3/SDL_init.h>
#include "internal_utils.h"
#include <SDL3_image/SDL_image.h>

static auto surfaceDeleter = [](SDL_Surface *s) {
    SIMPLE_2D_LOG_DEBUG << "Destroy SDL_Surface " << s;
    SDL_DestroySurface(s);
};

static auto textureDeleter = [](SDL_Texture *t) {
    SIMPLE_2D_LOG_DEBUG << "Destroy SDL_Texture " << t;
    SDL_DestroyTexture(t);
};

simple_2d::GraphicsSubsystem::GraphicsSubsystem() : mWindow(nullptr), mRenderer(nullptr), mWindowSize() {}

simple_2d::Error simple_2d::GraphicsSubsystem::Init(const std::string window_title, size_t window_width, size_t window_height, Color background_color) {
    SIMPLE_2D_LOG_INFO << "Initializing graphics subsystem...";
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SIMPLE_2D_LOG_ERROR << "Got error " << SDL_GetError() << "when try to init graphics!";
        return simple_2d::Error::INIT;
    }
    if (!SDL_CreateWindowAndRenderer(window_title.c_str(), window_width, window_height, 0, &mWindow, &mRenderer)) {
        SIMPLE_2D_LOG_ERROR << "Cannot create window and renderer!";
        return simple_2d::Error::INIT;
    }
    if (!SDL_GetWindowSize(mWindow, &mWindowSize.width, &mWindowSize.height)) {
        SIMPLE_2D_LOG_ERROR << "Something wrong when try to get window size!";
        return simple_2d::Error::INIT;
    }
    SDL_SetRenderDrawColor(mRenderer, background_color.r, background_color.g, background_color.b, background_color.a);
    SIMPLE_2D_LOG_INFO << "Graphics subsystem initialized successfully with renderer: " << mRenderer << " and window: " << mWindow;
    return simple_2d::Error::OK;
}

void simple_2d::GraphicsSubsystem::Deinit() {
    SIMPLE_2D_LOG_INFO << "Deinitializing graphics subsystem with renderer: " << mRenderer << " and window: " << mWindow;
    SDL_DestroyRenderer(mRenderer);
    SIMPLE_2D_LOG_DEBUG << "Destroyed renderer";
    SDL_DestroyWindow(mWindow);
    SIMPLE_2D_LOG_DEBUG << "Destroyed window";
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SIMPLE_2D_LOG_INFO << "Graphics subsystem deinitialized successfully!";
}

simple_2d::BitmapBundle simple_2d::GraphicsSubsystem::LoadImageFromFile(const std::string &path) {
    SIMPLE_2D_LOG_INFO << "Loading image file " << path;
    auto fullImagePath = GetRootPath() /= std::filesystem::path(path);
    auto loadedSurface = IMG_Load(fullImagePath.string().c_str());
    BitmapBundle ret = {
        .surface = nullptr,
        .texture = nullptr,
    };
    if (nullptr == loadedSurface) {
        SIMPLE_2D_LOG_ERROR << "Failed to load image file " << path;
        return ret;
    }
    auto texture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
    if (nullptr == texture) {
        SIMPLE_2D_LOG_ERROR << "Failed to create texture from surface! Get error: " << SDL_GetError();
        return ret;
    }
    ret.surface = std::shared_ptr<SDL_Surface>(loadedSurface, surfaceDeleter);
    ret.texture = std::shared_ptr<SDL_Texture>(texture, textureDeleter);
    SIMPLE_2D_LOG_DEBUG << "Loaded surface: " << ret.surface << " texture: " << ret.texture;
    SIMPLE_2D_LOG_INFO << "Loaded image file " << path;
    return ret;
}

simple_2d::Error simple_2d::GraphicsSubsystem::ClearRenderBuffer() {
    SIMPLE_2D_LOG_DEBUG << "Clearing render buffer";
    if (!SDL_RenderClear(mRenderer)) {
        SIMPLE_2D_LOG_ERROR << "Failed to clear render buffer! Get error: \"" << SDL_GetError() << "\"";
        return simple_2d::Error::RENDER;
    }
    return simple_2d::Error::OK;
}

simple_2d::Error simple_2d::GraphicsSubsystem::PutTextureToBackBuffer(const ManagedTexture &texture, XYCoordinate<float> pos) {
    SDL_FRect dst = {
        .x = pos.x,
        .y = pos.y,
        .w = (float) texture->w,
        .h = (float) texture->h,
    };
    SIMPLE_2D_LOG_DEBUG << "Putting texture to back buffer " << texture << " " << dst.x << " " << dst.y << " " << dst.w << " " << dst.h;
    SIMPLE_2D_LOG_DEBUG << "Renderer: " << mRenderer;
    if (!SDL_RenderTexture(mRenderer, texture.get(), NULL, &dst)) {
        SIMPLE_2D_LOG_ERROR << "Failed to put texture to back buffer! Get error: \"" << SDL_GetError() << "\"";
        return simple_2d::Error::RENDER;
    }
    return simple_2d::Error::OK;
}


simple_2d::Error simple_2d::GraphicsSubsystem::RenderBackBuffer() {
    SIMPLE_2D_LOG_DEBUG << "Rendering back buffer";
    if (!SDL_RenderPresent(mRenderer)) {
        SIMPLE_2D_LOG_ERROR << "Failed to render back buffer! Get error: \"" << SDL_GetError() << "\"";
        return simple_2d::Error::RENDER;
    }
    return simple_2d::Error::OK;
}

simple_2d::ManagedSurface simple_2d::GraphicsSubsystem::CreateBlankSurfaceFromDimensions(int width, int height, SDL_PixelFormat format) {
    return std::shared_ptr<SDL_Surface>(SDL_CreateSurface(width, height, format), surfaceDeleter);
}

simple_2d::ManagedTexture simple_2d::GraphicsSubsystem::CreateTextureFromSurface(ManagedSurface surface) {
    return std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(mRenderer, surface.get()), textureDeleter);
}
