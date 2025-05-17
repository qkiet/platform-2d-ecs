#include <simple-2d/core.h>
#include <boost/log/trivial.hpp>
#include <simple-2d/entity.h>

simple_2d::Engine::Engine() : mGraphics(), mAudio() {
    // Well, this is so weird. Have to call this boost log or well it will throw error "Failed to set TLS value: Invalid argument"
    // @todo: figure out why this is happening
    BOOST_LOG_TRIVIAL(debug) << "Engine constructor " << this;
}

simple_2d::Error simple_2d::Engine::Init(const std::string window_title, size_t window_width, size_t window_height, Color background_color) {
    if (mGraphics.Init(window_title, window_width, window_height, background_color) != Error::OK) {
        return Error::INIT;
    }
    if (mAudio.Init() != Error::OK) {
        return Error::INIT;
    }
    std::vector<std::string> component_names = {"platform_player", "downward_gravity", "static_sprite", "motion", "animated_sprite"};
    for (auto& component_name : component_names) {
        mComponentManagers[component_name] = std::make_shared<ComponentManager>();
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
    BOOST_LOG_TRIVIAL(debug) << "Stepping component manager platform_player";
    mComponentManagers["platform_player"]->Step();
    BOOST_LOG_TRIVIAL(debug) << "Stepping component manager downward_gravity";
    mComponentManagers["downward_gravity"]->Step();
    BOOST_LOG_TRIVIAL(debug) << "Stepping component manager static_sprite";
    mComponentManagers["static_sprite"]->Step();
    BOOST_LOG_TRIVIAL(debug) << "Stepping component manager motion";
    mComponentManagers["motion"]->Step();
    BOOST_LOG_TRIVIAL(debug) << "Stepping component manager animated_sprite";
    mComponentManagers["animated_sprite"]->Step();
    BOOST_LOG_TRIVIAL(debug) << "Stepping component managers done";
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

simple_2d::Error simple_2d::Engine::RegisterPlatformPlayerEventCallback(SDL_EventType eventType, ComponentEventsCallback callback, EntityId entityId) {
    auto component = mComponentManagers["platform_player"]->GetComponent(entityId);
    if (component == nullptr) {
        return Error::NOT_EXISTS;
    }
    mOnEventsCallbacksMap[eventType].push_back({callback, component});
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

std::shared_ptr<simple_2d::ComponentManager> simple_2d::Engine::GetComponentManager(const std::string& componentName) const {
    auto it = mComponentManagers.find(componentName);
    if (it == mComponentManagers.end()) {
        BOOST_LOG_TRIVIAL(error) << "Component manager not found: " << componentName;
        return nullptr;
    }
    return it->second;
}
