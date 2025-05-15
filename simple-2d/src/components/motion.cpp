#include <simple-2d/components/motion.h>
#include <boost/log/trivial.hpp>

simple_2d::MotionComponent::MotionComponent(): mPosition(0, 0), mVelocity(0, 0), mAcceleration(0, 0) {
    BOOST_LOG_TRIVIAL(debug) << "MotionComponent constructor " << this;
    mEntityId = 0;
    mIsEntityIdSet = false;
}

simple_2d::MotionComponent::~MotionComponent() {
    BOOST_LOG_TRIVIAL(debug) << "MotionComponent destructor " << this;
}

void simple_2d::MotionComponent::SetPosition(XYCoordinate<float> position) {
    mPosition = position;
}

simple_2d::XYCoordinate<float> simple_2d::MotionComponent::GetPosition() const {
    return mPosition;
}


void simple_2d::MotionComponent::SetVelocity(XYCoordinate<float> velocity) {
    mVelocity = velocity;
}

simple_2d::XYCoordinate<float> simple_2d::MotionComponent::GetVelocity() const {
    return mVelocity;
}

void simple_2d::MotionComponent::SetAcceleration(XYCoordinate<float> acceleration) {
    mAcceleration = acceleration;
}

simple_2d::XYCoordinate<float> simple_2d::MotionComponent::GetAcceleration() const {
    return mAcceleration;
}


simple_2d::Error simple_2d::MotionComponent::Step() {
    mPosition += mVelocity;
    mVelocity += mAcceleration;
    return simple_2d::Error::OK;
}
