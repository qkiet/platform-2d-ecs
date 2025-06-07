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
        static std::shared_ptr<Component> CreateComponent(std::string componentName, EntityId entityId);
    protected:
        // This field is used by component itself to locate other components with same entity id. For example, a game object
        // that has sprite component might want to know motion component of itself to know where to draw the sprite.
        EntityId mEntityId;
        // @deprecated Don't know what to do with this field. It's supposed to check whether the entity id is set or not. But now
        // entity id is set in the constructor of the component and cannot be changed throughout component lifetime, so this field
        // is not used.
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
        // How component manager process each tick is different. For example, most components only loop through all components
        // and call their Step() method. But some components, like collison body, will have special logic that call each component's
        // Step() method will result in lower performance. So this method is designed to be virtual and can be overridden by subclasses.
        virtual void Step() = 0;
        void RemoveEntity(EntityId id);
    protected:
        std::map<EntityId, std::shared_ptr<Component>> mComponents;
    };
}; // simple_2d


#endif // SIMPLE_2D_COMPONENT_H
