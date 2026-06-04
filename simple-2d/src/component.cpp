#include <simple-2d/component.h>
#include <simple-2d/utils.h>
#include <simple-2d/components/animated_sprite.h>
#include <simple-2d/components/motion.h>
#include <simple-2d/components/downward_gravity.h>
#include <simple-2d/components/static_sprite.h>
#include <simple-2d/components/json.h>
#include <simple-2d/components/behavior_script.h>
#include <simple-2d/components/static_repetitive_sprite.h>
#include <simple-2d/components/collision_body.h>

simple_2d::ComponentManager::ComponentManager() {
    SIMPLE_2D_LOG_DEBUG << "ComponentManager constructor " << this;
}

simple_2d::ComponentManager::~ComponentManager() {
    SIMPLE_2D_LOG_DEBUG << "ComponentManager destructor " << this;
}

void simple_2d::ComponentManager::SetName(std::string name) {
    mComponentManagerName = name;
    SIMPLE_2D_LOG_INFO << "Set component manager " << this << " name to " << name;
}

void simple_2d::ComponentManager::Step() {
    SIMPLE_2D_LOG_INFO << "Stepping component manager \"" << mComponentManagerName << "\"";
    DoStep();
}

std::shared_ptr<simple_2d::Component> simple_2d::Component::CreateComponent(ComponentType componentType, EntityId entityId) {
    switch (componentType)
    {
    case ANIMATED_SPITE:
        return std::make_shared<simple_2d::AnimatedSprite>(entityId);
    case BEHAVIOR_SCRIPT:
        return std::make_shared<simple_2d::BehaviorScript>(entityId);
    case COLLISION_BODY:
        return std::make_shared<simple_2d::CollisionBodyComponent>(entityId);
    case DOWNWARD_GRAVITY:
        return std::make_shared<simple_2d::DownwardGravity>(entityId);
    case STATIC_SPRITE:
        return std::make_shared<simple_2d::StaticSpriteComponent>(entityId);
    case STATIC_REPETITIVE_SPRITE:
        return std::make_shared<simple_2d::StaticRepetitiveSpriteComponent>(entityId);
    case MOTION:
        return std::make_shared<simple_2d::MotionComponent>(entityId);
    case JSON:
        return std::make_shared<simple_2d::JsonComponent>(entityId);
    default:
        SIMPLE_2D_LOG_ERROR << "Component type " << componentType << " not supported!";
        return nullptr;
    }
}

void simple_2d::Component::SetEntityId(EntityId id) {
    if (mIsEntityIdSet) {
        SIMPLE_2D_LOG_ERROR << "Entity ID is already set";
        return;
    }
    mEntityId = id;
    mIsEntityIdSet = true;
}

simple_2d::EntityId simple_2d::Component::GetEntityId() const {
    return mEntityId;
}

void simple_2d::ComponentManager::RegisterNewEntity(simple_2d::EntityId id, std::shared_ptr<Component> component) {
    mComponents[id] = component;
}

std::shared_ptr<simple_2d::Component> simple_2d::ComponentManager::GetComponent(EntityId id) const {
    auto it = mComponents.find(id);
    if (it == mComponents.end()) {
        SIMPLE_2D_LOG_ERROR << "Not found component \"" << mComponentManagerName << "\" for entity " << id;
        return nullptr;
    }
    return it->second;
}
