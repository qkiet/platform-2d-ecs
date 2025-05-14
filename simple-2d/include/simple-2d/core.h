#ifndef SIMPLE_2D_ENGINE_CORE_H
#define SIMPLE_2D_ENGINE_CORE_H
#include "graphics.h"
#include "error_type.h"
#include "generic_types.h"
#include "audio.h"

namespace simple_2d {
    class Engine {
    private:
        GraphicsSubsystem mGraphics;
        AudioSubsystem mAudio;
    public:
        Engine();
        // Initialize engine with given window title, window size and background color. Separate constructor and initialization is
        // deliberate choice because initialization may fail and we cannot handle failure in constructor.
        Error Init(const std::string window_title, size_t window_width, size_t window_height, Color background_color);

        // Process one tick of the engine.
        void Step();

    };
};


#endif
