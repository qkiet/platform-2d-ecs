#include <simple-2d/components/animated_sprite.h>
#include <boost/log/trivial.hpp>
#include <simple-2d/core.h>
#include <simple-2d/components/motion.h>

simple_2d::AnimatedSprite::AnimatedSprite(EntityId entityId) {
    mEntityId = entityId;
}

void simple_2d::AnimatedSprite::AddAnimation(AnimationId animation_id, ManagedTexture texture, int frameLengthTicks) {
    BOOST_LOG_TRIVIAL(debug) << "Adding animation " << animation_id << " with texture " << texture << " and frame length " << frameLengthTicks;
    mAnimationTree[animation_id].push_back({texture, frameLengthTicks});
}

simple_2d::Error simple_2d::AnimatedSprite::PlayAnimation(AnimationId animation_id) {
    if (mAnimationTree.find(animation_id) == mAnimationTree.end()) {
        BOOST_LOG_TRIVIAL(error) << "Animation not found";
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
        BOOST_LOG_TRIVIAL(error) << "Failed to render current frame";
        return err;
    }
    err = UpdateAnimation();
    if (Error::OK != err) {
        BOOST_LOG_TRIVIAL(error) << "Failed to update animation";
        return err;
    }
    return Error::OK;
}

simple_2d::Error simple_2d::AnimatedSprite::RenderCurrentFrame() const {
    if (mAnimationTree.find(mStatus.animation_id) == mAnimationTree.end()) {
        BOOST_LOG_TRIVIAL(error) << "Animation not found";
        return Error::NOT_EXISTS;
    }
    auto animation = mAnimationTree.at(mStatus.animation_id);
    if (mStatus.frame_id >= animation.size()) {
        BOOST_LOG_TRIVIAL(error) << "Frame id out of bounds";
        return Error::NOT_EXISTS;
    }
    auto frame = animation.at(mStatus.frame_id).texture;
    auto componentManager = simple_2d::Engine::GetInstance().GetComponentManager("motion");
    if (componentManager == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "Failed to get motion component manager";
        return Error::NOT_EXISTS;
    }
    auto component = componentManager->GetComponent(mEntityId);
    if (component == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "Failed to get motion component";
        return Error::NOT_EXISTS;
    }
    auto motionComponent = std::static_pointer_cast<MotionComponent>(component);
    auto position = motionComponent->GetPosition() + mOffset;
    Engine::GetInstance().GetGraphics().PutTextureToBackBuffer(frame, position);
    return Error::OK;
}

simple_2d::Error simple_2d::AnimatedSprite::UpdateAnimation() {
    if (mAnimationTree.find(mStatus.animation_id) == mAnimationTree.end()) {
        BOOST_LOG_TRIVIAL(error) << "Animation not found";
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
