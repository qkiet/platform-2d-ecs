#include <simple-2d/components/static_repetitive_sprite.h>
#include <simple-2d/core.h>
#include <simple-2d/components/motion.h>
#include <simple-2d/graphics.h>
#include <simple-2d/error_type.h>
#include <boost/log/trivial.hpp>
#include <SDL3/SDL.h>

static auto textureDeleter = [](SDL_Texture *t) {
    BOOST_LOG_TRIVIAL(debug) << "Destroy SDL_Texture " << t;
    SDL_DestroyTexture(t);
};

simple_2d::StaticRepetitiveSpriteComponent::StaticRepetitiveSpriteComponent(EntityId entityId) : mNeedsRebuildTexture(false) {
    mEntityId = entityId;
}

simple_2d::StaticRepetitiveSpriteComponent::StaticRepetitiveSpriteComponent(
    EntityId entityId, ManagedSurface surface, XYCoordinate<float> position) : mNeedsRebuildTexture(false), mOffset(position), mUnitSurface(surface) {
    mEntityId = entityId;
}

simple_2d::StaticRepetitiveSpriteComponent::StaticRepetitiveSpriteComponent(
    EntityId entityId, ManagedSurface surface, XYCoordinate<float> position, RectangularDimensions<int> dimensions)
    : mNeedsRebuildTexture(false), mOffset(position), mDimensions(dimensions), mUnitSurface(surface) {
    mEntityId = entityId;
}

void simple_2d::StaticRepetitiveSpriteComponent::SetUnitSurface(ManagedSurface surface) {
    mUnitSurface = surface;
    mNeedsRebuildTexture = true;
    mBuiltTexture = nullptr;
}

simple_2d::ManagedSurface simple_2d::StaticRepetitiveSpriteComponent::GetUnitSurface() const {
    return mUnitSurface;
}

void simple_2d::StaticRepetitiveSpriteComponent::SetDimensions(RectangularDimensions<int> dimensions) {
    mDimensions = dimensions;
    mNeedsRebuildTexture = true;
    mBuiltTexture = nullptr;
}

simple_2d::RectangularDimensions<int> simple_2d::StaticRepetitiveSpriteComponent::GetDimensions() const {
    return mDimensions;
}


void simple_2d::StaticRepetitiveSpriteComponent::SetOffset(XYCoordinate<float> offset) {
    mOffset = offset;
}

simple_2d::XYCoordinate<float> simple_2d::StaticRepetitiveSpriteComponent::GetOffset() const {
    return mOffset;
}


simple_2d::Error simple_2d::StaticRepetitiveSpriteComponent::Step() {
    if (mNeedsRebuildTexture) {
        RebuildTexture();
        mNeedsRebuildTexture = false;
    }
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
    simple_2d::Engine::GetInstance().PrepareTextureForRendering(mBuiltTexture, position);
    return Error::OK;
}

void simple_2d::StaticRepetitiveSpriteComponent::RebuildTexture() {
    auto graphics = simple_2d::Engine::GetInstance().GetGraphics();
    auto surface = graphics.CreateBlankSurfaceFromDimensions(mDimensions.width, mDimensions.height, SDL_PIXELFORMAT_RGBA8888);
    auto needPaddingExtraUnitSurfaceColumn = mDimensions.width % mUnitSurface->w == 0 ? false : true;
    auto needPaddingExtraUnitSurfaceRow = mDimensions.height % mUnitSurface->h == 0 ? false : true;
    auto numUnitSurfacesColumns = mDimensions.width / mUnitSurface->w + int(needPaddingExtraUnitSurfaceColumn);
    auto numUnitSurfacesRows = mDimensions.height / mUnitSurface->h + int(needPaddingExtraUnitSurfaceRow);
    for (auto i = 0; i < numUnitSurfacesColumns; i++) {
        for (auto j = 0; j < numUnitSurfacesRows; j++) {
            SDL_Rect dstrect = {i * mUnitSurface->w, j * mUnitSurface->h, mUnitSurface->w, mUnitSurface->h};
            SDL_BlitSurface(mUnitSurface.get(), nullptr, surface.get(), &dstrect);
        }
    }
    mBuiltTexture = graphics.CreateTextureFromSurface(surface);
}
