#ifndef SIMPLE_2D_ENGINE_CORE_H
#define SIMPLE_2D_ENGINE_CORE_H
#include "graphics.h"
#include "error_type.h"
#include "generic_types.h"
#include "audio.h"
#include "components/static_sprite.h"
#include "components/motion.h"

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
        ComponentManager mMotionComponentManager;

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

        /**
         * @brief Processes a single tick of the engine.
         *
         * This method advances the engine's state by one tick, updating all subsystems accordingly.
         */
        void Step();

        static Engine& GetInstance();


        GraphicsSubsystem& GetGraphics();

        AudioSubsystem& GetAudio();

        ComponentManager& GetStaticSpriteComponentManager();

        ComponentManager& GetMotionComponentManager();
    };
};


#endif
