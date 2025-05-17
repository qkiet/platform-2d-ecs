#include <simple-2d/component.h>
#include <boost/log/trivial.hpp>
#include <simple-2d/components/animated_sprite.h>
#include <simple-2d/components/motion.h>
#include <simple-2d/components/platform_player.h>
#include <simple-2d/components/downward_gravity.h>
#include <simple-2d/components/static_sprite.h>

simple_2d::ComponentManager::ComponentManager() {
    BOOST_LOG_TRIVIAL(debug) << "ComponentManager constructor " << this;
}

simple_2d::ComponentManager::~ComponentManager() {
    BOOST_LOG_TRIVIAL(debug) << "ComponentManager destructor " << this;
}

std::shared_ptr<simple_2d::Component> simple_2d::Component::CreateComponent(std::string componentName) {
    if (componentName == "animated_sprite") {
        return std::make_shared<simple_2d::AnimatedSprite>();
    }
    if (componentName == "motion") {
        return std::make_shared<simple_2d::MotionComponent>();
    }
    if (componentName == "platform_player") {
        return std::make_shared<simple_2d::PlatformPlayer>();
    }
    if (componentName == "downward_gravity") {
        return std::make_shared<simple_2d::DownwardGravity>();
    }
    if (componentName == "static_sprite") {
        return std::make_shared<simple_2d::StaticSpriteComponent>();
    }
    return nullptr;
}

void simple_2d::Component::SetEntityId(EntityId id) {
    if (mIsEntityIdSet) {
        BOOST_LOG_TRIVIAL(error) << "Entity ID is already set";
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
        return nullptr;
    }
    return it->second;
}

void simple_2d::ComponentManager::Step() {
    BOOST_LOG_TRIVIAL(debug) << "Stepping component manager " << this;
    for (auto& [entity, component] : mComponents) {
        BOOST_LOG_TRIVIAL(debug) << "Stepping entity " << entity << " with component " <<  component;
        component->Step();
    }
}

