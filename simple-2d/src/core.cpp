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
    return Error::OK;
}

void simple_2d::Engine::Deinit() {
    mGraphics.Deinit();
    mAudio.Deinit();
    SDL_Quit();
    BOOST_LOG_TRIVIAL(info) << "Engine deinitialized successfully!";
}

simple_2d::Error simple_2d::Engine::Step() {
    auto error = pollEvents();
    if (error == Error::QUIT) {
        return error;
    }
    // It's OK that engine is not initialized with a scene. So we don't need to stop the game if there is no scene.
    if (mCurrentScene == nullptr) {
        return Error::OK;
    }
    mGraphics.ClearRenderBuffer();
    mCurrentScene->Step();
    mAudio.PeriodicCleanUp();
    mGraphics.RenderBackBuffer();
    return Error::OK;
}

simple_2d::Error simple_2d::Engine::pollEvents() {
    SDL_Event event;
    mEvents.clear();
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            BOOST_LOG_TRIVIAL(info) << "Receive quit";
            return Error::QUIT;
        }
        if (event.type != SDL_EVENT_POLL_SENTINEL) {
            mEvents.push_back(event);
        }
    }
    return Error::OK;
}


std::vector<SDL_Event> simple_2d::Engine::GetEvents() const {
    return mEvents;
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

simple_2d::Error simple_2d::Engine::SetCurrentScene(std::shared_ptr<Scene> scene) {
    mCurrentScene = scene;
    // The scene can be initialized outside of engine, but we need to make sure it is initialized here or
    // unexpected behavior may happen.
    if (Error::OK != mCurrentScene->Init()) {
        BOOST_LOG_TRIVIAL(error) << "Failed to set and initialize scene";
        return Error::INIT;
    }
    return Error::OK;
}

std::shared_ptr<simple_2d::Scene> simple_2d::Engine::GetCurrentScene() const {
    return mCurrentScene;
}

std::shared_ptr<simple_2d::ComponentManager> simple_2d::Engine::GetComponentManager(const std::string& componentName) const {
    return mCurrentScene->GetComponentManager(componentName);
}

simple_2d::Camera& simple_2d::Engine::GetCamera() {
    return mCamera;
}

simple_2d::Error simple_2d::Engine::PrepareTextureForRendering(const ManagedTexture &texture, XYCoordinate<float> pos) {
    auto translatedPosition = pos - mCamera.GetPosition();
    return mGraphics.PutTextureToBackBuffer(texture, translatedPosition);
}
