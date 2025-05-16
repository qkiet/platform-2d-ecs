#include <simple-2d/components/downward_gravity.h>
#include <simple-2d/core.h>
#include <simple-2d/components/motion.h>
#include <boost/log/trivial.hpp>
#define DEFAULT_GRAVITY 0.2


simple_2d::Error simple_2d::DownwardGravity::Step() {
    auto [err, component] = simple_2d::Engine::GetInstance().GetMotionComponentManager().GetComponent(mEntityId);
    if (simple_2d::Error::OK != err) {
        BOOST_LOG_TRIVIAL(error) << "Failed to get motion component";
        return err;
    }
    auto motionComponent = std::static_pointer_cast<MotionComponent>(component);
    motionComponent->SetAccelerationOneAxis(Axis::Y, DEFAULT_GRAVITY);
    return Error::OK;
}
