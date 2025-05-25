#include <simple-2d/camera.h>

void simple_2d::Camera::SetPosition(XYCoordinate<float> position) {
    mPosition = position;
}

void simple_2d::Camera::SetDimensions(RectangularDimensions<int> dimensions) {
    mDimensions = dimensions;
}

simple_2d::XYCoordinate<float> simple_2d::Camera::GetPosition() const {
    return mPosition;
}

simple_2d::RectangularDimensions<int> simple_2d::Camera::GetDimensions() const {
    return mDimensions;
}