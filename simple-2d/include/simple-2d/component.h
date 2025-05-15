#ifndef SIMPLE_2D_COMPONENT_H
#define SIMPLE_2D_COMPONENT_H
#include <cstdint>
#include <memory>

namespace simple_2d {

    typedef uint64_t EntityId;

    class Component {
    public:
        virtual void Step() = 0;
    };

    class ComponentManager {
    public:
        virtual void RegisterNewEntity(EntityId id, std::shared_ptr<Component> component) = 0;
        virtual void Step() = 0;
        virtual void RemoveEntity(EntityId id) = 0;
    };
}; // simple_2d


#endif // SIMPLE_2D_COMPONENT_H
