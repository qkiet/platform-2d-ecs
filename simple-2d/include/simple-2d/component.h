#ifndef SIMPLE_2D_COMPONENT_H
#define SIMPLE_2D_COMPONENT_H
#include <cstdint>
#include <memory>
#include <utility>
#include "error_type.h"
#include <map>
#include <vector>
#include <SDL3/SDL_events.h>
#include <functional>
#include "generic_types.h"

namespace simple_2d {

    class Component {
    public:
        void SetEntityId(EntityId id);
        EntityId GetEntityId() const;
        virtual Error Step() = 0;
        static std::shared_ptr<Component> CreateComponent(std::string componentName);
    protected:
        EntityId mEntityId;
        bool mIsEntityIdSet;
    };

    // Event handler callback for component. Using reference to vector of events to avoid copying because engine has a
    // lot of components and it's expensive to copy the vector of events for each component.
    typedef std::function<void(std::shared_ptr<Component>, const SDL_Event&)> ComponentEventCallback;

    class ComponentManager {
    public:
        ComponentManager();
        ~ComponentManager();
        void RegisterNewEntity(EntityId id, std::shared_ptr<Component> component);
        std::shared_ptr<Component> GetComponent(EntityId id) const;
        void Step();
        void RemoveEntity(EntityId id);
    private:
        std::map<EntityId, std::shared_ptr<Component>> mComponents;
    };
}; // simple_2d


#endif // SIMPLE_2D_COMPONENT_H
