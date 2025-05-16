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
        ~MotionComponent();
        void SetPosition(XYCoordinate<float> position);
        void SetPositionOneAxis(Axis axis, float position);
        XYCoordinate<float> GetPosition() const;
        float GetPositionOneAxis(Axis axis) const;
        void IncrementPosition(XYCoordinate<float> position);
        void IncrementPositionOneAxis(Axis axis, float position);
        void SetVelocity(XYCoordinate<float> velocity);
        void SetVelocityOneAxis(Axis axis, float velocity);
        XYCoordinate<float> GetVelocity() const;
        float GetVelocityOneAxis(Axis axis) const;
        void IncrementVelocity(XYCoordinate<float> velocity);
        void IncrementVelocityOneAxis(Axis axis, float velocity);
        void SetAcceleration(XYCoordinate<float> acceleration);
        void SetAccelerationOneAxis(Axis axis, float acceleration);
        XYCoordinate<float> GetAcceleration() const;
        float GetAccelerationOneAxis(Axis axis) const;
        void IncrementAcceleration(XYCoordinate<float> acceleration);
        void IncrementAccelerationOneAxis(Axis axis, float acceleration);
        Error Step() override;
    private:
        XYCoordinate<float> mPosition;
        XYCoordinate<float> mVelocity;
        XYCoordinate<float> mAcceleration;
    };
}






#endif // SIMPLE_2D_MOTION_H
