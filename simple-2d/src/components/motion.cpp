#include <simple-2d/components/motion.h>
#include <boost/log/trivial.hpp>

simple_2d::MotionComponent::MotionComponent(EntityId entityId): mPosition(0, 0), mVelocity(0, 0), mAcceleration(0, 0) {
    BOOST_LOG_TRIVIAL(debug) << "MotionComponent constructor " << this;
    mEntityId = entityId;
}

simple_2d::MotionComponent::~MotionComponent() {
    BOOST_LOG_TRIVIAL(debug) << "MotionComponent destructor " << this;
}

void simple_2d::MotionComponent::SetPosition(XYCoordinate<float> position) {
    mPosition = position;
}

void simple_2d::MotionComponent::SetPositionOneAxis(Axis axis, float position) {
    switch (axis) {
        case Axis::X:
            mPosition.x = position;
            break;
        case Axis::Y:
            mPosition.y = position;
    }
}

void simple_2d::MotionComponent::IncrementPosition(XYCoordinate<float> position) {
    mPosition += position;
}

void simple_2d::MotionComponent::IncrementPositionOneAxis(Axis axis, float position) {
    switch (axis) {
        case Axis::X:
            mPosition.x += position;
            break;
        case Axis::Y:
            mPosition.y += position;
    }
}
simple_2d::XYCoordinate<float> simple_2d::MotionComponent::GetPosition() const {
    return mPosition;
}

float simple_2d::MotionComponent::GetPositionOneAxis(Axis axis) const {
    float ret = 0;
    switch (axis) {
        case Axis::X:
            ret = mPosition.x;
        case Axis::Y:
            ret = mPosition.y;
    }
    return ret;
}

void simple_2d::MotionComponent::SetVelocity(XYCoordinate<float> velocity) {
    mVelocity = velocity;
}

void simple_2d::MotionComponent::SetVelocityOneAxis(Axis axis, float velocity) {
    switch (axis) {
        case Axis::X:
            mVelocity.x = velocity;
            break;
        case Axis::Y:
            mVelocity.y = velocity;
    }
}


simple_2d::XYCoordinate<float> simple_2d::MotionComponent::GetVelocity() const {
    return mVelocity;
}

float simple_2d::MotionComponent::GetVelocityOneAxis(Axis axis) const {
    float ret = 0;
    switch (axis) {
        case Axis::X:
            ret = mVelocity.x;
        case Axis::Y:
            ret = mVelocity.y;
    }
    return ret;
}

void simple_2d::MotionComponent::IncrementVelocity(XYCoordinate<float> velocity) {
    mVelocity += velocity;
}

void simple_2d::MotionComponent::IncrementVelocityOneAxis(Axis axis, float velocity) {
    switch (axis) {
        case Axis::X:
            mVelocity.x += velocity;
    }
}

void simple_2d::MotionComponent::SetAcceleration(XYCoordinate<float> acceleration) {
    mAcceleration = acceleration;
}

simple_2d::XYCoordinate<float> simple_2d::MotionComponent::GetAcceleration() const {
    return mAcceleration;
}

float simple_2d::MotionComponent::GetAccelerationOneAxis(Axis axis) const {
    float ret = 0;
    switch (axis) {
        case Axis::X:
            ret = mAcceleration.x;
        case Axis::Y:
            ret = mAcceleration.y;
    }
    return ret;
}
void simple_2d::MotionComponent::IncrementAcceleration(XYCoordinate<float> acceleration) {
    mAcceleration += acceleration;
}

void simple_2d::MotionComponent::SetAccelerationOneAxis(Axis axis, float acceleration) {
    switch (axis) {
        case Axis::X:
            mAcceleration.x = acceleration;
            break;
        case Axis::Y:
            mAcceleration.y = acceleration;
    }
}

simple_2d::Error simple_2d::MotionComponent::Step() {
    BOOST_LOG_TRIVIAL(debug) << "MotionComponent step for entity " << mEntityId;
    BOOST_LOG_TRIVIAL(debug) << "Velocity: " << mVelocity;
    BOOST_LOG_TRIVIAL(debug) << "Acceleration: " << mAcceleration;
    BOOST_LOG_TRIVIAL(debug) << "Position: " << mPosition;
    mPosition += mVelocity;
    mVelocity += mAcceleration;
    return simple_2d::Error::OK;
}

void simple_2d::MotionComponentManager::Step() {
    for (auto &component : mComponents) {
        auto motionComponent = std::static_pointer_cast<MotionComponent>(component.second);
        motionComponent->Step();
    }
}
