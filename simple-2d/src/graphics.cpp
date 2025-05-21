#include <simple-2d/graphics.h>
#include <boost/log/trivial.hpp>
#include <SDL3/SDL_init.h>
#include "internal_utils.h"
#include <SDL3_image/SDL_image.h>

static auto surfaceDeleter = [](SDL_Surface *s) {
    BOOST_LOG_TRIVIAL(debug) << "Destroy SDL_Surface " << s;
    SDL_DestroySurface(s);
};

static auto textureDeleter = [](SDL_Texture *t) {
    BOOST_LOG_TRIVIAL(debug) << "Destroy SDL_Texture " << t;
    SDL_DestroyTexture(t);
};

simple_2d::GraphicsSubsystem::GraphicsSubsystem() : mWindow(nullptr), mRenderer(nullptr), mWindowSize() {}

simple_2d::Error simple_2d::GraphicsSubsystem::Init(const std::string window_title, size_t window_width, size_t window_height, Color background_color) {
    BOOST_LOG_TRIVIAL(info) << "Initializing graphics subsystem...";
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        BOOST_LOG_TRIVIAL(error) << "Got error " << SDL_GetError() << "when try to init graphics!";
        return simple_2d::Error::INIT;
    }
    if (!SDL_CreateWindowAndRenderer(window_title.c_str(), window_width, window_height, 0, &mWindow, &mRenderer)) {
        BOOST_LOG_TRIVIAL(error) << "Cannot create window and renderer!";
        return simple_2d::Error::INIT;
    }
    if (!SDL_GetWindowSize(mWindow, &mWindowSize.width, &mWindowSize.height)) {
        BOOST_LOG_TRIVIAL(error) << "Something wrong when try to get window size!";
        return simple_2d::Error::INIT;
    }
    SDL_SetRenderDrawColor(mRenderer, background_color.r, background_color.g, background_color.b, background_color.a);
    BOOST_LOG_TRIVIAL(info) << "Graphics subsystem initialized successfully with renderer: " << mRenderer << " and window: " << mWindow;
    return simple_2d::Error::OK;
}

void simple_2d::GraphicsSubsystem::Deinit() {
    BOOST_LOG_TRIVIAL(info) << "Deinitializing graphics subsystem with renderer: " << mRenderer << " and window: " << mWindow;
    SDL_DestroyRenderer(mRenderer);
    BOOST_LOG_TRIVIAL(debug) << "Destroyed renderer";
    SDL_DestroyWindow(mWindow);
    BOOST_LOG_TRIVIAL(debug) << "Destroyed window";
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    BOOST_LOG_TRIVIAL(info) << "Graphics subsystem deinitialized successfully!";
}

simple_2d::BitmapBundle simple_2d::GraphicsSubsystem::LoadImageFromFile(const std::string &path) {
    BOOST_LOG_TRIVIAL(info) << "Loading image file " << path;
    auto fullImagePath = GetRootPath() /= std::filesystem::path(path);
    auto loadedSurface = IMG_Load(fullImagePath.c_str());
    BitmapBundle ret = {
        .surface = nullptr,
        .texture = nullptr,
    };
    if (nullptr == loadedSurface) {
        BOOST_LOG_TRIVIAL(error) << "Failed to load image file " << path;
        return ret;
    }
    auto texture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
    if (nullptr == texture) {
        BOOST_LOG_TRIVIAL(error) << "Failed to create texture from surface! Get error: " << SDL_GetError();
        return ret;
    }
    ret.surface = std::shared_ptr<SDL_Surface>(loadedSurface, surfaceDeleter);
    ret.texture = std::shared_ptr<SDL_Texture>(texture, textureDeleter);
    BOOST_LOG_TRIVIAL(debug) << "Loaded surface: " << ret.surface << " texture: " << ret.texture;
    BOOST_LOG_TRIVIAL(info) << "Loaded image file " << path;
    return ret;
}

simple_2d::Error simple_2d::GraphicsSubsystem::ClearRenderBuffer() {
    BOOST_LOG_TRIVIAL(debug) << "Clearing render buffer";
    if (!SDL_RenderClear(mRenderer)) {
        BOOST_LOG_TRIVIAL(error) << "Failed to clear render buffer! Get error: \"" << SDL_GetError() << "\"";
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
    BOOST_LOG_TRIVIAL(debug) << "Putting texture to back buffer " << texture << " " << dst.x << " " << dst.y << " " << dst.w << " " << dst.h;
    BOOST_LOG_TRIVIAL(debug) << "Renderer: " << mRenderer;
    if (!SDL_RenderTexture(mRenderer, texture.get(), NULL, &dst)) {
        BOOST_LOG_TRIVIAL(error) << "Failed to put texture to back buffer! Get error: \"" << SDL_GetError() << "\"";
        return simple_2d::Error::RENDER;
    }
    return simple_2d::Error::OK;
}


simple_2d::Error simple_2d::GraphicsSubsystem::RenderBackBuffer() {
    BOOST_LOG_TRIVIAL(debug) << "Rendering back buffer";
    if (!SDL_RenderPresent(mRenderer)) {
        BOOST_LOG_TRIVIAL(error) << "Failed to render back buffer! Get error: \"" << SDL_GetError() << "\"";
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
