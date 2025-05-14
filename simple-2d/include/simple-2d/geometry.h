#ifndef SIMPLE_2D_ENGINE_GEOMETRY_H
#define SIMPLE_2D_ENGINE_GEOMETRY_H
#include <iostream>

namespace simple_2d {
    // XYCoordinate is a template class that represents a point in a 2D space.
    template<typename T>
    struct XYCoordinate {
        T x;
        T y;
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

    template<typename T>
    struct RectangularDimension {
        T height;
        T width;
    };

    enum Axis {
        X,
        Y,
    };

    template<typename T>
    struct Rectangle {
        XYCoordinate<T> top_left;
        XYCoordinate<T> bottom_right;
        template <typename T2>
        friend std::ostream &operator<<(std::ostream &os, Rectangle<T2> &rect) {
            os << "(" << rect.top_left.x << "," << rect.top_left.y << ")-(" << rect.bottom_right.x << "," << rect.bottom_right.y << ")";
            return os;
        }
    };

    template<typename T>
    bool AreRectanglesOverlap(Rectangle<T> rect1, Rectangle<T> rect2);
};

#endif // SIMPLE_2D_ENGINE_GEOMETRY_H
