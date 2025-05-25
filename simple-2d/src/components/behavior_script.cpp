#include <simple-2d/components/behavior_script.h>
#include <simple-2d/core.h>

void simple_2d::BehaviorScript::SetOnTickEventCallback(std::function<void(EntityId)> callback) {
    mOnTickCallback = callback;
}

void simple_2d::BehaviorScript::SetOnKeyPressedEventCallback(std::function<void(EntityId, const SDL_Event &)> callback) {
    mOnKeyPressedCallback = callback;
}

void simple_2d::BehaviorScript::SetOnKeyReleasedEventCallback(std::function<void(EntityId, const SDL_Event &)> callback) {
    mOnKeyReleasedCallback = callback;
}

simple_2d::BehaviorScript::BehaviorScript(EntityId entityId) {
    mEntityId = entityId;
}

simple_2d::Error simple_2d::BehaviorScript::Step() {
    // Handle all key events first before tick
    for (auto& event : Engine::GetInstance().GetEvents()) {
        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (mOnKeyPressedCallback) {
                mOnKeyPressedCallback(mEntityId, event);
            }
        } else if (event.type == SDL_EVENT_KEY_UP) {
            if (mOnKeyReleasedCallback) {
                mOnKeyReleasedCallback(mEntityId, event);
            }
        }
    }
    // Handle tick
    if (mOnTickCallback) {
        mOnTickCallback(mEntityId);
    }

    return Error::OK;
}


void simple_2d::BehaviorScriptComponentManager::Step() {
    for (auto &component : mComponents) {
        auto behaviorScript = std::static_pointer_cast<BehaviorScript>(component.second);
        behaviorScript->Step();
    }
}
