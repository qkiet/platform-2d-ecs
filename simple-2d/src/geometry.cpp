#include <simple-2d/geometry.h>

template<typename T>
bool simple_2d::AreRectanglesOverlap(Rectangle<T> rect1, Rectangle<T> rect2) {
    if (rect1.top_left.x > rect2.bottom_right.x || rect1.bottom_right.x < rect2.top_left.x) {
        return false;
    }
    if (rect1.top_left.y > rect2.bottom_right.y || rect1.bottom_right.y < rect2.top_left.y) {
        return false;
    }
    return true;
}

template bool simple_2d::AreRectanglesOverlap(Rectangle<float> rect1, Rectangle<float> rect2);

template<typename T>
simple_2d::AxisAlignedEdgesRelativePosition simple_2d::RelativePositionBetweenAxisAlignedEdges(AxisAlignedEdge<T> edge1, AxisAlignedEdge<T> edge2) {
    if (edge1.alignedAxis != edge2.alignedAxis) {
        throw std::invalid_argument("Edges are not parallel");
    }
    if (edge1.alignedAxis == Axis::X) {
        if (edge1.origin.y < edge2.origin.y) {
            return AxisAlignedEdgesRelativePosition::Above;
        } else if (edge1.origin.y > edge2.origin.y) {
            return AxisAlignedEdgesRelativePosition::Below;
        } else {
            return AxisAlignedEdgesRelativePosition::Aligned;
        }
    }
    if (edge1.alignedAxis == Axis::Y) {
        if (edge1.origin.x < edge2.origin.x) {
            return AxisAlignedEdgesRelativePosition::LeftOf;
        } else if (edge1.origin.x > edge2.origin.x) {
            return AxisAlignedEdgesRelativePosition::RightOf;
        } else {
            return AxisAlignedEdgesRelativePosition::Aligned;
        }
    }
    return AxisAlignedEdgesRelativePosition::Intersecting;
}

template simple_2d::AxisAlignedEdgesRelativePosition simple_2d::RelativePositionBetweenAxisAlignedEdges(AxisAlignedEdge<float> edge1, AxisAlignedEdge<float> edge2);

template<typename T>
T simple_2d::GetDistanceBetweenAxisAlignedEdges(AxisAlignedEdge<T> edge1, AxisAlignedEdge<T> edge2) {
    if (edge1.alignedAxis != edge2.alignedAxis) {
        throw std::invalid_argument("Edges are not parallel");
    }
    if (edge1.alignedAxis == Axis::X) {
        return std::abs(edge2.origin.y - edge1.origin.y);
    }
    if (edge1.alignedAxis == Axis::Y) {
        return std::abs(edge2.origin.x - edge1.origin.x);
    }
    // Should not reach here, just to make compiler happy
    return 0;
}

template float simple_2d::GetDistanceBetweenAxisAlignedEdges(AxisAlignedEdge<float> edge1, AxisAlignedEdge<float> edge2);
