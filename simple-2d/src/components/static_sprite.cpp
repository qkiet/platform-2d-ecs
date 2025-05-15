#include <simple-2d/components/static_sprite.h>
#include <simple-2d/core.h>
#include <boost/log/trivial.hpp>
simple_2d::StaticSpriteComponent::StaticSpriteComponent() : mTexture(nullptr), mPosition(0, 0) {}
simple_2d::StaticSpriteComponent::StaticSpriteComponent(ManagedTexture texture) : mTexture(texture), mPosition(0, 0) {}
simple_2d::StaticSpriteComponent::StaticSpriteComponent(ManagedTexture texture, XYCoordinate<float> position) : mTexture(texture), mPosition(position) {}
void simple_2d::StaticSpriteComponent::SetTexture(ManagedTexture texture) {
    mTexture = texture;
}
void simple_2d::StaticSpriteComponent::SetPosition(XYCoordinate<float> position) {
    mPosition = position;
}
simple_2d::ManagedTexture simple_2d::StaticSpriteComponent::GetTexture() const {
    return mTexture;
}
simple_2d::XYCoordinate<float> simple_2d::StaticSpriteComponent::GetPosition() const {
    return mPosition;
}

void simple_2d::StaticSpriteComponent::Step() {
    BOOST_LOG_TRIVIAL(debug) << "Stepping static sprite " << mTexture << " " << mPosition.x << " " << mPosition.y;
    BOOST_LOG_TRIVIAL(debug) << "engine ins from static sprite: " << &simple_2d::Engine::GetInstance();
    BOOST_LOG_TRIVIAL(debug) << "Graphics ins from static sprite: " << &simple_2d::Engine::GetInstance().GetGraphics();
    simple_2d::Engine::GetInstance().GetGraphics().PutTextureToBackBuffer(mTexture, mPosition);
}

void simple_2d::StaticSpriteComponentManager::RegisterNewEntity(EntityId id, std::shared_ptr<Component> component) {
    mSprites[id] = std::static_pointer_cast<StaticSpriteComponent>(component);
}

void simple_2d::StaticSpriteComponentManager::Step() {
    BOOST_LOG_TRIVIAL(debug) << "Stepping static sprite manager";
    for (auto const& sprite : mSprites)
    {
        sprite.second->Step();
    }
}

void simple_2d::StaticSpriteComponentManager::RemoveEntity(EntityId id) {
    mSprites.erase(id);
}
