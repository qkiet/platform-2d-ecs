#include <simple-2d/core.h>
#include <boost/log/trivial.hpp>

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
    mStaticSpriteComponentManager.Step();
    mMotionComponentManager.Step();
    mAudio.PeriodicCleanUp();
    mGraphics.RenderBackBuffer();
}


simple_2d::Engine& simple_2d::Engine::GetInstance() {
    static Engine instance;
    return instance;
}

simple_2d::GraphicsSubsystem& simple_2d::Engine::GetGraphics() {
    return mGraphics;
}

simple_2d::AudioSubsystem& simple_2d::Engine::GetAudio() {
    return mAudio;
}

simple_2d::ComponentManager& simple_2d::Engine::GetStaticSpriteComponentManager() {
    return mStaticSpriteComponentManager;
}


simple_2d::ComponentManager& simple_2d::Engine::GetMotionComponentManager() {
    return mMotionComponentManager;
}
