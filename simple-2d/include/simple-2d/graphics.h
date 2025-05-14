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

    /**
     * @class GraphicsSubsystem
     * @brief Manages graphics operations including window creation, rendering, and texture management.
     *
     * This class provides an interface to initialize the graphics subsystem, manage rendering buffers,
     * and handle textures. It uses SDL for rendering operations.
     */
    class GraphicsSubsystem {
    private:
        SDL_Window *mWindow; ///< Pointer to the SDL window.
        SDL_Renderer *mRenderer; ///< Pointer to the SDL renderer.
        RectangularDimension<int> mWindowSize; ///< Dimensions of the window.
    public:
        /**
         * @brief Constructs the GraphicsSubsystem object.
         */
        GraphicsSubsystem();

        /**
         * @brief Destructor to clean up resources.
         */
        ~GraphicsSubsystem();

        /**
         * @brief Initializes the graphics subsystem with specified parameters.
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
         * @brief Clears the render buffer.
         *
         * This method clears the current rendering target with the background color.
         */
        void ClearRenderBuffer();

        /**
         * @brief Puts a texture onto the back buffer at a specified position.
         *
         * @param texture The texture to render.
         * @param pos The position to render the texture at.
         */
        void PutTextureToBackBuffer(const ManagedTexture &texture, XYCoordinate<float> pos);

        /**
         * @brief Renders the back buffer to the screen.
         *
         * This method presents the current rendering target to the display.
         */
        void RenderBackBuffer();

        /**
         * @brief Deinitializes the graphics subsystem.
         *
         * This method cleans up resources used by the graphics subsystem.
         */
        void Deinit();
    };

}; // simple_2d

#endif // SIMPLE_2D_GRAPHICS_H
