#include <simple-2d/core.h>
#include <boost/log/trivial.hpp>

simple_2d::Engine::Engine() : mGraphics() {
}

simple_2d::Error simple_2d::Engine::Init(const std::string window_title, size_t window_width, size_t window_height, Color background_color) {
    if (mGraphics.Init(window_title, window_width, window_height, background_color) != Error::OK) {
        return Error::INIT;
    }
    if (mAudio.Init() != Error::OK) {
        return Error::INIT;
    }
    return Error::OK;
}

void simple_2d::Engine::Deinit() {
    mGraphics.Deinit();
    mAudio.Deinit();
    SDL_Quit();
    BOOST_LOG_TRIVIAL(info) << "Engine deinitialized successfully!";
}

simple_2d::Error simple_2d::Engine::Step() {
    mGraphics.ClearRenderBuffer();
    auto [error, events] = GetEvents();
    for (auto& event : events) {
        BOOST_LOG_TRIVIAL(debug) << "Event: " << event.type;
    }
    if (error == Error::QUIT) {
        return error;
    }
    for (auto& event : events) {
        for (auto& eventCallback : mOnEventsCallbacksMap[(SDL_EventType)event.type]) {
            eventCallback.callback(eventCallback.component, event);
        }
    }
    mPlatformPlayerComponentManager.Step();
    mDownwardGravityComponentManager.Step();
    mStaticSpriteComponentManager.Step();
    mMotionComponentManager.Step();
    mAudio.PeriodicCleanUp();
    mGraphics.RenderBackBuffer();
    return Error::OK;
}

std::pair<simple_2d::Error, std::vector<SDL_Event>> simple_2d::Engine::GetEvents() {
    SDL_Event event;
    std::vector<SDL_Event> ret;
    SDL_PollEvent(&event);
    if (event.type == SDL_EVENT_QUIT) {
        BOOST_LOG_TRIVIAL(info) << "Receive quit";
        return {Error::QUIT, {}};
    }
    if (event.type != SDL_EVENT_POLL_SENTINEL) {
        ret.push_back(event);
    }
    return {Error::OK, ret};
}

simple_2d::Error simple_2d::Engine::RegisterPlatformPlayerEventCallback(SDL_EventType event_type, ComponentEventsCallback callback, EntityId entity_id) {
    auto [err, component] = mPlatformPlayerComponentManager.GetComponent(entity_id);
    if (Error::OK != err) {
        return err;
    }
    mOnEventsCallbacksMap[event_type].push_back({callback, component});
    return Error::OK;
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

simple_2d::ComponentManager& simple_2d::Engine::GetDownwardGravityComponentManager() {
    return mDownwardGravityComponentManager;
}

simple_2d::ComponentManager& simple_2d::Engine::GetStaticSpriteComponentManager() {
    return mStaticSpriteComponentManager;
}


simple_2d::ComponentManager& simple_2d::Engine::GetMotionComponentManager() {
    return mMotionComponentManager;
}

simple_2d::ComponentManager& simple_2d::Engine::GetPlatformPlayerComponentManager() {
    return mPlatformPlayerComponentManager;
}
