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

std::shared_ptr<simple_2d::Component> simple_2d::Component::CreateComponent(std::string componentName, EntityId entityId) {
    if (componentName == "animated_sprite") {
        return std::make_shared<simple_2d::AnimatedSprite>(entityId);
    }
    if (componentName == "motion") {
        return std::make_shared<simple_2d::MotionComponent>(entityId);
    }
    if (componentName == "downward_gravity") {
        return std::make_shared<simple_2d::DownwardGravity>(entityId);
    }
    if (componentName == "static_sprite") {
        return std::make_shared<simple_2d::StaticSpriteComponent>(entityId);
    }
    if (componentName == "json") {
        return std::make_shared<simple_2d::JsonComponent>(entityId);
    }
    if (componentName == "behavior_script") {
        return std::make_shared<simple_2d::BehaviorScript>(entityId);
    }
    if (componentName == "static_repetitive_sprite") {
        return std::make_shared<simple_2d::StaticRepetitiveSpriteComponent>(entityId);
    }
    if (componentName == "collision_body") {
        return std::make_shared<simple_2d::CollisionBodyComponent>(entityId);
    }
    SIMPLE_2D_LOG_ERROR << "Failed to create component " << componentName;
    return nullptr;
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
        SIMPLE_2D_LOG_ERROR << "Component not found for entity " << id;
        return nullptr;
    }
    return it->second;
}
