#ifndef SIMPLE_2D_GRAPHICS_H
#define SIMPLE_2D_GRAPHICS_H
#include "error_type.h"

#include <memory>
#include <vector>
#include <map>
#include <utility>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_render.h>
#include "geometry.h"
#include "generic_types.h"



namespace simple_2d {
    // Using std::shared_ptr to circumvent problem of copy assignment when pushing stuff into vector...
    typedef std::shared_ptr<SDL_Surface> ManagedSurface;

    typedef std::shared_ptr<SDL_Texture> ManagedTexture;

    class GraphicsSubsystem {
    private:
        SDL_Window *mWindow;
        SDL_Renderer *mRenderer;
        RectangularDimension<int> mWindowSize;
    public:
        GraphicsSubsystem();
        ~GraphicsSubsystem();
        // Initialize graphics subsystem with given window title, window size and background color. Separate constructor and initialization is
        // deliberate choice because initialization may fail and we cannot handle failure in constructor.
        Error Init(const std::string window_title, size_t window_width, size_t window_height, Color background_color);
        void ClearRenderBuffer();
        void PutTextureToBackBuffer( const ManagedTexture &texture, XYCoordinate<float> pos);
        void RenderBackBuffer();
        void Deinit();
    };

}; // simple_2d

#endif // SIMPLE_2D_GRAPHICS_H
