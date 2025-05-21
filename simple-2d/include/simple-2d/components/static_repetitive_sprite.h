#ifndef SIMPLE_2D_COMPONENT_STATIC_REPETITIVE_SPRITE_H
#define SIMPLE_2D_COMPONENT_STATIC_REPETITIVE_SPRITE_H

#include <simple-2d/component.h>
#include <simple-2d/geometry.h>
#include <simple-2d/graphics.h>

namespace simple_2d {
    class StaticRepetitiveSpriteComponent: public Component {
    public:
        StaticRepetitiveSpriteComponent(EntityId entityId);
        StaticRepetitiveSpriteComponent(EntityId entityId, ManagedSurface surface, XYCoordinate<float> position);
        StaticRepetitiveSpriteComponent(EntityId entityId, ManagedSurface surface, XYCoordinate<float> position, RectangularDimensions<int> dimensions);
        ~StaticRepetitiveSpriteComponent() = default;
        void SetUnitSurface(ManagedSurface surface);
        ManagedSurface GetUnitSurface() const;
        void SetDimensions(RectangularDimensions<int> dimensions);
        RectangularDimensions<int> GetDimensions() const;
        void SetOffset(XYCoordinate<float> offset);
        XYCoordinate<float> GetOffset() const;
        Error Step() override;
    private:
        XYCoordinate<float> mOffset;
        RectangularDimensions<int> mDimensions;
        ManagedSurface mUnitSurface;
        ManagedTexture mBuiltTexture;
        bool mNeedsRebuildTexture;
        void RebuildTexture();
    };
}; // simple_2d

#endif // SIMPLE_2D_COMPONENT_STATIC_REPETITIVE_SPRITE_H
