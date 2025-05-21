#ifndef SIMPLE_2D_COMPONENTS_BEHAVIOR_SCRIPT_H
#define SIMPLE_2D_COMPONENTS_BEHAVIOR_SCRIPT_H
#include <simple-2d/component.h>
#include <simple-2d/entity.h>
#include <functional>

namespace simple_2d {

    class BehaviorScript : public Component {
    public:
        BehaviorScript(EntityId entityId);
        ~BehaviorScript() = default;
        void SetOnTickEventCallback(std::function<void(EntityId)> callback);
        void SetOnKeyPressedEventCallback(std::function<void(EntityId, const SDL_Event &)> callback);
        void SetOnKeyReleasedEventCallback(std::function<void(EntityId, const SDL_Event &)> callback);
        Error Step() override;
    private:
        std::function<void(EntityId)> mOnTickCallback;
        std::function<void(EntityId, const SDL_Event &)> mOnKeyPressedCallback;
        std::function<void(EntityId, const SDL_Event &)> mOnKeyReleasedCallback;
    };
}

#endif // SIMPLE_2D_COMPONENTS_BEHAVIOR_SCRIPT_H
