#include <simple-2d/component.h>
#include <boost/log/trivial.hpp>

simple_2d::ComponentManager::ComponentManager() {
    BOOST_LOG_TRIVIAL(debug) << "ComponentManager constructor " << this;
}

simple_2d::ComponentManager::~ComponentManager() {
    BOOST_LOG_TRIVIAL(debug) << "ComponentManager destructor " << this;
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

void simple_2d::ComponentManager::RegisterNewEntity(EntityId id, std::shared_ptr<Component> component) {
    mComponents[id] = component;
}

std::pair<simple_2d::Error, std::shared_ptr<simple_2d::Component>> simple_2d::ComponentManager::GetComponent(EntityId id) {
    auto it = mComponents.find(id);
    if (it == mComponents.end()) {
        return std::make_pair(simple_2d::Error::NOT_EXISTS, nullptr);
    }
    return std::make_pair(simple_2d::Error::OK, it->second);
}

void simple_2d::ComponentManager::Step() {
    for (auto& component : mComponents) {
        component.second->Step();
    }
}

