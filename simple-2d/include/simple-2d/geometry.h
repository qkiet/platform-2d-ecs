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
        XYCoordinate() : x(0), y(0) {}
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

        template <typename T2>
        friend std::ostream &operator<<(std::ostream &os, const XYCoordinate<T2> &c);
    };

    template <typename T>
    std::ostream &operator<<(std::ostream &os, const XYCoordinate<T> &c) {
        os << "(" << c.x << "," << c.y << ")";
        return os;
    }

    template <typename T>
    XYCoordinate<T> operator+(const XYCoordinate<T> &c1, const XYCoordinate<T> &c2) {
        XYCoordinate<T> res;
        res.x = c1.x + c2.x;
        res.y = c1.y + c2.y;
        return res;
    }

    template <typename T>
    XYCoordinate<T> operator-(const XYCoordinate<T> &c1, const XYCoordinate<T> &c2) {
        XYCoordinate<T> res;
        res.x = c1.x - c2.x;
        res.y = c1.y - c2.y;
        return res;
    }

    /**
     * @struct RectangularDimensions
     * @brief Represents the dimensions of a rectangle.
     *
     * @tparam T The data type of the dimensions.
     */
    template<typename T>
    struct RectangularDimensions {
        T width; ///< Width of the rectangle. X axis
        T height; ///< Height of the rectangle. Y axis
        RectangularDimensions() : width(0), height(0) {}
        RectangularDimensions(T width, T height) : width(width), height(height) {}
        template <typename T2>
        operator XYCoordinate<T2>() const {
            return XYCoordinate<T2>(width, height);
        }
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
     * @struct AxisAlignedEdge
     * @brief Represents an axis-aligned edge
     *
     * @tparam T The data type of the coordinates.
     */
    template<typename T>
    struct AxisAlignedEdge {
        // The origin is the point where the edge starts. Note that the direction from origin to end of edge is:
        // - If alignedAxis is Axis::X, then the direction is from left to right.
        // - If alignedAxis is Axis::Y, then the direction is from top to bottom.
        XYCoordinate<T> origin;
        // The alignedAxis is the axis that the edge is aligned with.
        Axis alignedAxis;
        // The length is the distance from the origin to the end of the edge.
        T length;
        AxisAlignedEdge() : origin(0, 0), alignedAxis(Axis::X), length(0) {}
        AxisAlignedEdge(XYCoordinate<T> origin, Axis alignedAxis, T length) : origin(origin), alignedAxis(alignedAxis), length(length) {}
        template <typename T2>
        friend std::ostream &operator<<(std::ostream &os, const AxisAlignedEdge<T2> &edge);
    };

    template <typename T>
    std::ostream &operator<<(std::ostream &os, const AxisAlignedEdge<T> &edge) {
        os << "(" << edge.origin.x << "," << edge.origin.y << ")-(" << edge.origin.x + edge.length * (edge.alignedAxis == Axis::X ? 1 : 0) << "," << edge.origin.y + edge.length * (edge.alignedAxis == Axis::Y ? 1 : 0) << "," << (edge.alignedAxis == Axis::X ? 'X' : 'Y') << ")";
        return os;
    }

    enum RectangleEdge {
        Top, ///< Top edge of the rectangle.
        Bottom, ///< Bottom edge of the rectangle.
        Left, ///< Left edge of the rectangle.
        Right, ///< Right edge of the rectangle.
    };
    /**
     * @struct Rectangle. Note that this type is AXIS-ALIGNED (which means that the edges are all parallel to the axes).
     * @brief Represents a rectangle defined by two coordinates.
     *
     * @tparam T The data type of the coordinates.
     */
    template<typename T>
    struct Rectangle {
        XYCoordinate<T> top_left; ///< Top-left coordinate of the rectangle.
        XYCoordinate<T> bottom_right; ///< Bottom-right coordinate of the rectangle.
        template <typename T2>
        friend std::ostream &operator<<(std::ostream &os, const Rectangle<T2> &rect);
        template <typename T2>
        AxisAlignedEdge<T2> GetAxisAlignedEdge(RectangleEdge edge) {
            switch (edge) {
                case RectangleEdge::Top:
                    return AxisAlignedEdge<T2>(top_left, Axis::X, bottom_right.y - top_left.y);
                case RectangleEdge::Bottom:
                    return AxisAlignedEdge<T2>(XYCoordinate<T2>(top_left.x, bottom_right.y), Axis::X, bottom_right.y - top_left.y);
                case RectangleEdge::Left:
                    return AxisAlignedEdge<T2>(top_left, Axis::Y, bottom_right.x - top_left.x);
                case RectangleEdge::Right:
                    return AxisAlignedEdge<T2>(XYCoordinate<T2>(bottom_right.x, top_left.y), Axis::Y, bottom_right.x - top_left.x);
                default:
                    throw std::invalid_argument("Invalid edge");
            }
        }
    };

    template <typename T2>
    std::ostream &operator<<(std::ostream &os, const Rectangle<T2> &rect) {
        os << "(" << rect.top_left.x << "," << rect.top_left.y << ")-(" << rect.bottom_right.x << "," << rect.bottom_right.y << ")";
        return os;
    }

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

    enum AxisAlignedEdgesRelativePosition {
        Above, ///< 1st edge is above the 2nd edge.
        Below, ///< 1st edge is below the 2nd edge.
        LeftOf, ///< 1st edge is to the left of the 2nd edge.
        RightOf, ///< 1st edge is to the right of the 2nd edge.
        Aligned, ///< 1st edge is aligned with the 2nd edge.
        Intersecting, ///< 1st edge is intersecting the 2nd edge.
    };

    /**
     * @brief Returns the relative position of two parallel edges. Beware that this functions ASSUMES that the edges are parallel and axis-aligned.
     *
     * @tparam T The data type of the coordinates.
     * @param edge1 The first edge.
     * @param edge2 The second edge.
     * @return The relative position of the two edges.
     */
    template<typename T>
    AxisAlignedEdgesRelativePosition RelativePositionBetweenAxisAlignedEdges(AxisAlignedEdge<T> edge1, AxisAlignedEdge<T> edge2);

    template<typename T>
    T GetDistanceBetweenAxisAlignedEdges(AxisAlignedEdge<T> edge1, AxisAlignedEdge<T> edge2);
};

#endif // SIMPLE_2D_ENGINE_GEOMETRY_H
