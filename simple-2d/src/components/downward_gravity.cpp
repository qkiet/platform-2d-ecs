#include <simple-2d/components/downward_gravity.h>
#include <simple-2d/core.h>
#include <simple-2d/components/motion.h>
#include <simple-2d/utils.h>
#define DEFAULT_GRAVITY 0.2

simple_2d::DownwardGravity::DownwardGravity(EntityId entityId) {
    mEntityId = entityId;
}

simple_2d::Error simple_2d::DownwardGravity::Step() {
    auto componentManager = simple_2d::Engine::GetInstance().GetComponentManager("motion");
    if (componentManager == nullptr) {
        SIMPLE_2D_LOG_ERROR << "Failed to get motion component manager";
        return Error::NOT_EXISTS;
    }
    auto component = componentManager->GetComponent(mEntityId);
    if (component == nullptr) {
        SIMPLE_2D_LOG_ERROR << "Failed to get motion component for entity " << mEntityId;
        return Error::NOT_EXISTS;
    }
    auto motionComponent = std::static_pointer_cast<MotionComponent>(component);
    motionComponent->SetAccelerationOneAxis(Axis::Y, DEFAULT_GRAVITY);
    return Error::OK;
}

void simple_2d::DownwardGravityComponentManager::Step() {
    for (auto &component : mComponents) {
        auto downwardGravity = std::static_pointer_cast<DownwardGravity>(component.second);
        downwardGravity->Step();
    }
}