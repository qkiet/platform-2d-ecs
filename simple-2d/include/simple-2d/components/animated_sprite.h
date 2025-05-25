#ifndef SIMPLE_2D_COMPONENT_ANIMATED_SPRITE_H
#define SIMPLE_2D_COMPONENT_ANIMATED_SPRITE_H

#include <simple-2d/component.h>
#include <simple-2d/graphics.h>

namespace simple_2d {
    typedef uint16_t AnimationId;

    class AnimatedSprite : public Component {
    public:
        AnimatedSprite(EntityId entityId);
        ~AnimatedSprite() = default;
        void AddAnimation(AnimationId animationId, ManagedTexture texture, int frameLengthTicks);
        Error PlayAnimation(AnimationId animationId);
        Error Step() override;
        void SetOffset(XYCoordinate<float> offset);
        XYCoordinate<float> GetOffset() const;
    private:

        struct AnimatedFrame {
            ManagedTexture texture;
            int frame_length_ticks;
        };

        typedef std::vector<AnimatedFrame> AnimatedFrames;

        typedef std::map<AnimationId, AnimatedFrames> AnimationTree;

        struct Status {
            AnimationId animation_id;
            int frame_id;
            int frame_ticks;
        };
        AnimationTree mAnimationTree;
        Status mStatus;
        XYCoordinate<float> mOffset;
        Error RenderCurrentFrame() const;
        Error UpdateAnimation();
    };

    class AnimatedSpriteComponentManager : public ComponentManager {
    public:
        AnimatedSpriteComponentManager() = default;
        ~AnimatedSpriteComponentManager() = default;
        void Step() override;
    };
}
#endif // SIMPLE_2D_COMPONENT_ANIMATED_SPRITE_H
