#ifndef SIMPLE_2D_ENGINE_GEOMETRY_H
#define SIMPLE_2D_ENGINE_GEOMETRY_H
#include <iostream>

namespace simple_2d {
    /**
     * @struct XYCoordinate
     * @brief Represents a point in a 2D space.
     *
     * @tparam T The data type of the coordinates.
     */
    template<typename T>
    struct XYCoordinate {
        T x; ///< X-coordinate of the point.
        T y; ///< Y-coordinate of the point.
        XYCoordinate(T x, T y) : x(x), y(y) {}
        XYCoordinate& operator+=(const XYCoordinate &c) {
            this->x += c.x;
            this->y += c.y;
            return *this;
        }
        XYCoordinate& operator-=(const XYCoordinate &c) {
            this->x -= c.x;
            this->y -= c.y;
            return *this;
        }

        XYCoordinate operator+(const XYCoordinate &c) {
            XYCoordinate res;
            res.x = x + c.x;
            res.y = y + c.y;
            return res;
        }

        XYCoordinate operator-(const XYCoordinate &c) {
            XYCoordinate res;
            res.x = x - c.x;
            res.y = y - c.y;
            return res;
        }

        template <typename T2>
        friend std::ostream &operator<<(std::ostream &os, const XYCoordinate<T2> &c) {
            os << "(" << c.x << "," << c.y << ")";
            return os;
        }
    };

    /**
     * @struct RectangularDimension
     * @brief Represents the dimensions of a rectangle.
     *
     * @tparam T The data type of the dimensions.
     */
    template<typename T>
    struct RectangularDimension {
        T height; ///< Height of the rectangle.
        T width; ///< Width of the rectangle.
    };

    /**
     * @enum Axis
     * @brief Represents the axis in a 2D space.
     */
    enum Axis {
        X, ///< X-axis.
        Y, ///< Y-axis.
    };

    /**
     * @struct Rectangle
     * @brief Represents a rectangle defined by two coordinates.
     *
     * @tparam T The data type of the coordinates.
     */
    template<typename T>
    struct Rectangle {
        XYCoordinate<T> top_left; ///< Top-left coordinate of the rectangle.
        XYCoordinate<T> bottom_right; ///< Bottom-right coordinate of the rectangle.
        template <typename T2>
        friend std::ostream &operator<<(std::ostream &os, Rectangle<T2> &rect) {
            os << "(" << rect.top_left.x << "," << rect.top_left.y << ")-(" << rect.bottom_right.x << "," << rect.bottom_right.y << ")";
            return os;
        }
    };

    /**
     * @brief Checks if two rectangles overlap.
     *
     * @tparam T The data type of the coordinates.
     * @param rect1 The first rectangle.
     * @param rect2 The second rectangle.
     * @return True if the rectangles overlap, false otherwise.
     */
    template<typename T>
    bool AreRectanglesOverlap(Rectangle<T> rect1, Rectangle<T> rect2);
};

#endif // SIMPLE_2D_ENGINE_GEOMETRY_H
