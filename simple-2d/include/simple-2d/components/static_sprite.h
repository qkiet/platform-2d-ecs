#ifndef SIMPLE_2D_COMPONENT_SPRITE_H
#define SIMPLE_2D_COMPONENT_SPRITE_H

#include <simple-2d/geometry.h>
#include <simple-2d/graphics.h>
#include <simple-2d/component.h>
#include <map>

namespace simple_2d {
    class StaticSpriteComponent: public Component {
    public:
        StaticSpriteComponent();
        StaticSpriteComponent(ManagedTexture bundle);
        StaticSpriteComponent(ManagedTexture texture, XYCoordinate<float> position);
        ~StaticSpriteComponent() = default;
        void SetTexture(ManagedTexture texture);
        void SetPosition(XYCoordinate<float> position);
        ManagedTexture GetTexture() const;
        XYCoordinate<float> GetPosition() const;
        void Step();
    private:
        XYCoordinate<float> mPosition;
        ManagedTexture mTexture;
    };

    class StaticSpriteComponentManager: public ComponentManager {

    private:
        std::map<EntityId, std::shared_ptr<StaticSpriteComponent>> mSprites;
    public:
        StaticSpriteComponentManager() = default;
        void RegisterNewEntity(EntityId id, std::shared_ptr<Component> component);
        void Step();
        void RemoveEntity(EntityId id);
    };
}; // simple_2d

#endif // SIMPLE_2D_COMPONENT_SPRITE_H
