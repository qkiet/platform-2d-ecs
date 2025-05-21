#include <simple-2d/components/static_sprite.h>
#include <simple-2d/core.h>
#include <simple-2d/components/motion.h>
#include <boost/log/trivial.hpp>

simple_2d::StaticSpriteComponent::StaticSpriteComponent(EntityId entityId) : mTexture(nullptr), mOffset(0, 0) {
    BOOST_LOG_TRIVIAL(debug) << "StaticSpriteComponent constructor " << this;
    mEntityId = entityId;
}
simple_2d::StaticSpriteComponent::StaticSpriteComponent(EntityId entityId, ManagedTexture texture) : mTexture(texture), mOffset(0, 0) {
    BOOST_LOG_TRIVIAL(debug) << "StaticSpriteComponent constructor " << this;
    mEntityId = 0;
}
simple_2d::StaticSpriteComponent::StaticSpriteComponent(EntityId entityId, ManagedTexture texture, XYCoordinate<float> position) :
        mTexture(texture), mOffset(position) {
    BOOST_LOG_TRIVIAL(debug) << "StaticSpriteComponent constructor " << this;
    mEntityId = entityId;
}

void simple_2d::StaticSpriteComponent::SetTexture(ManagedTexture texture) {
    mTexture = texture;
}
void simple_2d::StaticSpriteComponent::SetOffset(XYCoordinate<float> offset) {
    mOffset = offset;
}
simple_2d::ManagedTexture simple_2d::StaticSpriteComponent::GetTexture() const {
    return mTexture;
}
simple_2d::XYCoordinate<float> simple_2d::StaticSpriteComponent::GetOffset() const {
    return mOffset;
}

simple_2d::Error simple_2d::StaticSpriteComponent::Step() {
    auto componentManager = simple_2d::Engine::GetInstance().GetComponentManager("motion");
    if (componentManager == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "Failed to get motion component manager";
        return Error::NOT_EXISTS;
    }
    auto component = componentManager->GetComponent(mEntityId);
    if (component == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "Failed to get motion component for entity " << mEntityId;
        return Error::NOT_EXISTS;
    }
    auto positionComponent = std::static_pointer_cast<MotionComponent>(component);
    auto position = positionComponent->GetPosition() + mOffset;
    simple_2d::Engine::GetInstance().GetGraphics().PutTextureToBackBuffer(mTexture, position);
    return simple_2d::Error::OK;
}
