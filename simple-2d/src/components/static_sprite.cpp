#include <simple-2d/components/static_sprite.h>
#include <simple-2d/core.h>
#include <boost/log/trivial.hpp>
simple_2d::StaticSpriteComponent::StaticSpriteComponent() : mTexture(nullptr), mOffset(0, 0) {
    BOOST_LOG_TRIVIAL(debug) << "StaticSpriteComponent constructor " << this;
    mEntityId = 0;
    mIsEntityIdSet = false;
}
simple_2d::StaticSpriteComponent::StaticSpriteComponent(ManagedTexture texture) : mTexture(texture), mOffset(0, 0) {
    BOOST_LOG_TRIVIAL(debug) << "StaticSpriteComponent constructor " << this;
    mEntityId = 0;
    mIsEntityIdSet = false;
}
simple_2d::StaticSpriteComponent::StaticSpriteComponent(ManagedTexture texture, XYCoordinate<float> position) :
        mTexture(texture), mOffset(position) {
    BOOST_LOG_TRIVIAL(debug) << "StaticSpriteComponent constructor " << this;
    mEntityId = 0;
    mIsEntityIdSet = false;
}

void simple_2d::StaticSpriteComponent::SetTexture(ManagedTexture texture) {
    mTexture = texture;
}
void simple_2d::StaticSpriteComponent::SetPosition(XYCoordinate<float> position) {
    mOffset = position;
}
simple_2d::ManagedTexture simple_2d::StaticSpriteComponent::GetTexture() const {
    return mTexture;
}
simple_2d::XYCoordinate<float> simple_2d::StaticSpriteComponent::GetPosition() const {
    return mOffset;
}

simple_2d::Error simple_2d::StaticSpriteComponent::Step() {
    auto [err, component] = simple_2d::Engine::GetInstance().GetMotionComponentManager().GetComponent(mEntityId);
    if (simple_2d::Error::OK != err) {
        BOOST_LOG_TRIVIAL(error) << "Failed to get position component";
        return err;
    }
    auto positionComponent = std::static_pointer_cast<MotionComponent>(component);
    auto position = positionComponent->GetPosition() + mOffset;
    simple_2d::Engine::GetInstance().GetGraphics().PutTextureToBackBuffer(mTexture, position);
    return simple_2d::Error::OK;
}
