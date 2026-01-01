#include <simple-2d/components/animated_sprite.h>
#include <simple-2d/utils.h>
#include <simple-2d/core.h>
#include <simple-2d/components/motion.h>

simple_2d::AnimatedSprite::AnimatedSprite(EntityId entityId) {
    mEntityId = entityId;
}

void simple_2d::AnimatedSprite::AddAnimation(AnimationId animation_id, ManagedTexture texture, int frameLengthTicks) {
    SIMPLE_2D_LOG_DEBUG << "Adding animation " << animation_id << " with texture " << texture << " and frame length " << frameLengthTicks;
    mAnimationTree[animation_id].push_back({texture, frameLengthTicks});
}

simple_2d::Error simple_2d::AnimatedSprite::PlayAnimation(AnimationId animation_id) {
    if (mAnimationTree.find(animation_id) == mAnimationTree.end()) {
        SIMPLE_2D_LOG_ERROR << "Animation not found";
        return Error::NOT_EXISTS;
    }
    mStatus.animation_id = animation_id;
    mStatus.frame_id = 0;
    mStatus.frame_ticks = 0;
    return Error::OK;
}

simple_2d::Error simple_2d::AnimatedSprite::Step() {
    auto err = RenderCurrentFrame();
    if (Error::OK != RenderCurrentFrame()) {
        SIMPLE_2D_LOG_ERROR << "Failed to render current frame";
        return err;
    }
    err = UpdateAnimation();
    if (Error::OK != err) {
        SIMPLE_2D_LOG_ERROR << "Failed to update animation";
        return err;
    }
    return Error::OK;
}

simple_2d::Error simple_2d::AnimatedSprite::RenderCurrentFrame() const {
    if (mAnimationTree.find(mStatus.animation_id) == mAnimationTree.end()) {
        SIMPLE_2D_LOG_ERROR << "Animation not found";
        return Error::NOT_EXISTS;
    }
    auto animation = mAnimationTree.at(mStatus.animation_id);
    if (mStatus.frame_id >= animation.size()) {
        SIMPLE_2D_LOG_ERROR << "Frame id out of bounds";
        return Error::NOT_EXISTS;
    }
    auto frame = animation.at(mStatus.frame_id).texture;
    auto componentManager = simple_2d::Engine::GetInstance().GetComponentManager("motion");
    if (componentManager == nullptr) {
        SIMPLE_2D_LOG_ERROR << "Failed to get motion component manager";
        return Error::NOT_EXISTS;
    }
    auto component = componentManager->GetComponent(mEntityId);
    if (component == nullptr) {
        SIMPLE_2D_LOG_ERROR << "Failed to get motion component";
        return Error::NOT_EXISTS;
    }
    auto motionComponent = std::static_pointer_cast<MotionComponent>(component);
    auto position = motionComponent->GetPosition() + mOffset;
    Engine::GetInstance().PrepareTextureForRendering(frame, position);
    return Error::OK;
}

simple_2d::Error simple_2d::AnimatedSprite::UpdateAnimation() {
    if (mAnimationTree.find(mStatus.animation_id) == mAnimationTree.end()) {
        SIMPLE_2D_LOG_ERROR << "Animation not found";
        return Error::NOT_EXISTS;
    }
    mStatus.frame_ticks++;
    if (mStatus.frame_ticks >= mAnimationTree[mStatus.animation_id][mStatus.frame_id].frame_length_ticks) {
        mStatus.frame_id++;
        if (mStatus.frame_id >= mAnimationTree[mStatus.animation_id].size()) {
            mStatus.frame_id = 0;
        }
        mStatus.frame_ticks = 0;
    }
    return Error::OK;
}

void simple_2d::AnimatedSprite::SetOffset(XYCoordinate<float> offset) {
    mOffset = offset;
}

simple_2d::XYCoordinate<float> simple_2d::AnimatedSprite::GetOffset() const {
    return mOffset;
}

void simple_2d::AnimatedSpriteComponentManager::Step() {
    for (auto &component : mComponents) {
        auto animatedSprite = std::static_pointer_cast<AnimatedSprite>(component.second);
        animatedSprite->Step();
    }
}
