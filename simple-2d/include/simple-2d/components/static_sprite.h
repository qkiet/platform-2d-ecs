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
        Error Step() override;
    private:
        // Offset from the entity's position to the top-left corner of the sprite
        XYCoordinate<float> mOffset;
        ManagedTexture mTexture;
    };
}; // simple_2d

#endif // SIMPLE_2D_COMPONENT_SPRITE_H
