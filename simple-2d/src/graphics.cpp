#include <simple-2d/graphics.h>
#include <boost/log/trivial.hpp>
#include <SDL3/SDL_init.h>


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
    BOOST_LOG_TRIVIAL(info) << "Graphics subsystem initialized successfully!";
    return simple_2d::Error::OK;
}

simple_2d::GraphicsSubsystem::~GraphicsSubsystem() {
    BOOST_LOG_TRIVIAL(info) << "Deinitializing graphics subsystem...";
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    BOOST_LOG_TRIVIAL(info) << "Graphics subsystem deinitialized successfully!";
}

void simple_2d::GraphicsSubsystem::ClearRenderBuffer() {
    SDL_RenderClear(mRenderer);
}

void simple_2d::GraphicsSubsystem::PutTextureToBackBuffer(const ManagedTexture &texture, XYCoordinate<float> pos) {
}


void simple_2d::GraphicsSubsystem::RenderBackBuffer() {
    SDL_RenderPresent(mRenderer);
}

void simple_2d::GraphicsSubsystem::Deinit() {
}



