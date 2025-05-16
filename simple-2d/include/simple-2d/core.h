#ifndef SIMPLE_2D_ENGINE_CORE_H
#define SIMPLE_2D_ENGINE_CORE_H
#include "graphics.h"
#include "error_type.h"
#include "generic_types.h"
#include "audio.h"
#include "component.h"
#include <map>
#include <SDL3/SDL_events.h>

namespace simple_2d {
    /**
     * @class Engine
     * @brief Manages the core functionalities of the 2D engine, including graphics and audio subsystems.
     */
    class Engine {
    private:
        GraphicsSubsystem mGraphics; ///< Handles all graphics-related operations.
        AudioSubsystem mAudio; ///< Manages audio functionalities.
        ComponentManager mStaticSpriteComponentManager;
        ComponentManager mAnimatedSpriteComponentManager;
        ComponentManager mMotionComponentManager;
        ComponentManager mPlatformPlayerComponentManager;
        ComponentManager mDownwardGravityComponentManager;
        struct OnEventsCallback {
            ComponentEventsCallback callback;
            std::shared_ptr<Component> component;
        };
        std::vector<OnEventsCallback> mOnEventsCallbacks;
        std::map<SDL_EventType, std::vector<OnEventsCallback>> mOnEventsCallbacksMap;
        std::pair<Error, std::vector<SDL_Event>> GetEvents();

    public:
        /**
         * @brief Constructs the Engine object.
         */
        Engine();
        Engine(Engine const&) = delete;
        void operator=(Engine const&) = delete; // Don't implement
        // Initialize engine with given window title, window size and background color. Separate constructor and initialization is
        // deliberate choice because initialization may fail and we cannot handle failure in constructor.
        /**
         * @brief Initializes the engine with specified parameters.
         *
         * @param window_title The title of the window.
         * @param window_width The width of the window in pixels.
         * @param window_height The height of the window in pixels.
         * @param background_color The background color of the window.
         * @return Error indicating success or failure of the initialization.
         *
         * This method separates construction and initialization to handle potential initialization failures.
         */
        Error Init(const std::string window_title, size_t window_width, size_t window_height, Color background_color);

        void Deinit();

        /**
         * @brief Processes a single tick of the engine.
         *
         * This method advances the engine's state by one tick, updating all subsystems accordingly.
         */
        simple_2d::Error Step();

        static Engine& GetInstance();


        GraphicsSubsystem& GetGraphics();

        AudioSubsystem& GetAudio();

        ComponentManager& GetStaticSpriteComponentManager();

        ComponentManager& GetAnimatedSpriteComponentManager();

        ComponentManager& GetMotionComponentManager();

        ComponentManager& GetPlatformPlayerComponentManager();

        ComponentManager& GetDownwardGravityComponentManager();

        Error RegisterPlatformPlayerEventCallback(SDL_EventType event_type, ComponentEventsCallback callback, EntityId entity_id);
    };
};


#endif
