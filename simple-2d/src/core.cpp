#include <simple-2d/core.h>

simple_2d::Engine::Engine() : mGraphics() {}

simple_2d::Error simple_2d::Engine::Init(const std::string window_title, size_t window_width, size_t window_height, Color background_color) {
    if (mGraphics.Init(window_title, window_width, window_height, background_color) != Error::OK) {
        return Error::INIT;
    }
    if (mAudio.Init() != Error::OK) {
        return Error::INIT;
    }
    return Error::OK;
}

void simple_2d::Engine::Step() {
    mGraphics.ClearRenderBuffer();
    mGraphics.RenderBackBuffer();
    mAudio.PeriodicCleanUp();
}
