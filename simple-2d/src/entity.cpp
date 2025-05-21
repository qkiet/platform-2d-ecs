#include <simple-2d/entity.h>
#include <simple-2d/core.h>
#include <boost/log/trivial.hpp>

static simple_2d::EntityId currentEntityId = 0;

simple_2d::Entity::Entity() : mEntityId(currentEntityId++) {
    BOOST_LOG_TRIVIAL(info) << "Created entity " << mEntityId;
}

simple_2d::Entity::~Entity() {
}

simple_2d::EntityId simple_2d::Entity::GetEntityId() const {
    return mEntityId;
}

simple_2d::Error simple_2d::Entity::AddComponent(std::string componentName) {
    auto componentManager = Engine::GetInstance().GetComponentManager(componentName);
    if (componentManager == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "Failed to get component manager for component " << componentName;
        return Error::NOT_EXISTS;
    }
    auto component = Component::CreateComponent(componentName, mEntityId);
    if (component == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "Failed to create component " << componentName << " for entity " << mEntityId;
        return Error::NOT_EXISTS;
    }
    componentManager->RegisterNewEntity(mEntityId, component);
    BOOST_LOG_TRIVIAL(info) << "Added component " << componentName << " for entity " << mEntityId;
    return Error::OK;
}

std::shared_ptr<simple_2d::Component> simple_2d::Entity::GetComponent(std::string componentName) const {
    auto componentManager = Engine::GetInstance().GetComponentManager(componentName);
    if (componentManager == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "Failed to get component manager for component " << componentName;
        return nullptr;
    }
    return componentManager->GetComponent(mEntityId);
}
