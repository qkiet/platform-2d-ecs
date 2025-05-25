#ifndef SIMPLE_2D_CAMERA_H
#define SIMPLE_2D_CAMERA_H

#include "geometry.h"

namespace simple_2d {
    class Camera {
    private:
        XYCoordinate<float> mPosition;
        RectangularDimensions<int> mDimensions;
    public:
        Camera() = default;
        ~Camera() = default;
        void SetPosition(XYCoordinate<float> position);
        void SetDimensions(RectangularDimensions<int> dimensions);
        XYCoordinate<float> GetPosition() const;
        RectangularDimensions<int> GetDimensions() const;
    };
}
#endif // SIMPLE_2D_CAMERA_H