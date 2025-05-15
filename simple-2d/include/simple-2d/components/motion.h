#ifndef SIMPLE_2D_MOTION_H
#define SIMPLE_2D_MOTION_H
#include <simple-2d/component.h>
#include <simple-2d/geometry.h>
#include <map>
#include <memory>

namespace simple_2d {
    class MotionComponent: public Component {
    public:
        MotionComponent();
        ~MotionComponent() = default;
        void SetPosition(XYCoordinate<float> position);
        XYCoordinate<float> GetPosition() const;
        void SetVelocity(XYCoordinate<float> velocity);
        XYCoordinate<float> GetVelocity() const;
        void SetAcceleration(XYCoordinate<float> acceleration);
        XYCoordinate<float> GetAcceleration() const;
        Error Step();
    private:
        XYCoordinate<float> mPosition;
        XYCoordinate<float> mVelocity;
        XYCoordinate<float> mAcceleration;
    };
}






#endif // SIMPLE_2D_MOTION_H
