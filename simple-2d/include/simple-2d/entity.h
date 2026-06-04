#ifndef SIMPLE_2D_ENTITY_H
#define SIMPLE_2D_ENTITY_H
#include <cstdint>
#include "error_type.h"
#include <string>
#include <memory>
#include <map>
#include "component.h"
#include "generic_types.h"

namespace simple_2d {


    class Entity {
    public:
        Entity();
        ~Entity();
        EntityId GetEntityId() const;
        Error AddComponent(ComponentType componentType);
        std::shared_ptr<Component> GetComponent(ComponentType componentType) const;
    protected:
        EntityId mEntityId; // Entity ID is unique for each entity.
    };
}

#endif // SIMPLE_2D_ENTITY_H