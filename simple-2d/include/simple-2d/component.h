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

namespace simple_2d {

    typedef uint64_t EntityId;

    class Component {
    public:
        void SetEntityId(EntityId id);
        EntityId GetEntityId() const;
        virtual Error Step() = 0;
    protected:
        EntityId mEntityId;
        bool mIsEntityIdSet;
    };

    // Event handler callback for component. Using reference to vector of events to avoid copying because engine has a
    // lot of components and it's expensive to copy the vector of events for each component.
    typedef std::function<void(std::shared_ptr<Component>, const SDL_Event&)> ComponentEventsCallback;

    class ComponentManager {
    public:
        ComponentManager();
        ~ComponentManager();
        void RegisterNewEntity(EntityId id, std::shared_ptr<Component> component);
        std::pair<Error, std::shared_ptr<Component>> GetComponent(EntityId id);
        void Step();
        void RemoveEntity(EntityId id);
    private:
        std::map<EntityId, std::shared_ptr<Component>> mComponents;
    };
}; // simple_2d


#endif // SIMPLE_2D_COMPONENT_H
