#ifndef SIMPLE_2D_GENERIC_TYPES_H
#define SIMPLE_2D_GENERIC_TYPES_H

#include <cstdint>

namespace simple_2d {
    /**
     * @struct Color
     * @brief Represents a color with red, green, blue, and alpha components.
     */
    struct Color {
        uint8_t r; ///< Red component of the color
        uint8_t g; ///< Green component of the color
        uint8_t b; ///< Blue component of the color
        uint8_t a; ///< Alpha component of the color (transparency)
    };
}

#endif
