#include <simple-2d/components/platform_player.h>
#include <simple-2d/components/motion.h>
#include <boost/log/trivial.hpp>
#include <simple-2d/core.h>

// @todo: move to new config file stuff
#define MOVE_SPEED_PER_TICKS 10
#define JUMP_INITIAL_SPEED 15
#define GRAVITY 0.2


static void onKeyPressedEvent(std::shared_ptr<simple_2d::Component> component, const SDL_Event& event) {
    BOOST_LOG_TRIVIAL(debug) << "onKeyPressedEvent for key " << event.key.scancode << " for entity " << component->GetEntityId();
    auto player = std::static_pointer_cast<simple_2d::PlatformPlayer>(component);
    switch (event.key.scancode)
    {
    case SDL_SCANCODE_LEFT:
        player->SetIsMovingLeft(true);
        break;
    case SDL_SCANCODE_RIGHT:
        player->SetIsMovingRight(true);
        break;
    default:
        break;
    }
}

static void onKeyReleasedEvent(std::shared_ptr<simple_2d::Component> component, const SDL_Event& event) {
    BOOST_LOG_TRIVIAL(debug) << "onKeyReleasedEvent for key " << event.key.scancode << " for entity " << component->GetEntityId();
    auto player = std::static_pointer_cast<simple_2d::PlatformPlayer>(component);
    switch (event.key.scancode)
    {
    case SDL_SCANCODE_SPACE:
        player->SetWantToJump(true);
        break;
    case SDL_SCANCODE_LEFT:
        player->SetIsMovingLeft(false);
        break;
    case SDL_SCANCODE_RIGHT:
        player->SetIsMovingRight(false);
        break;
    default:
        break;
    }
}

void simple_2d::PlatformPlayer::RegisterEventsCallbacks() {
    simple_2d::Engine::GetInstance().RegisterPlatformPlayerEventCallback(SDL_EVENT_KEY_DOWN, onKeyPressedEvent, mEntityId);
    simple_2d::Engine::GetInstance().RegisterPlatformPlayerEventCallback(SDL_EVENT_KEY_UP, onKeyReleasedEvent, mEntityId);
}

simple_2d::Error simple_2d::PlatformPlayer::Step() {
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
    auto motionComponent = std::static_pointer_cast<MotionComponent>(component);
    int direction = (int)mIsMovingRight - (int)mIsMovingLeft;
    BOOST_LOG_TRIVIAL(debug) << "Direction: " << direction;
    int aboutToJump = mWantToJump && !mIsJumping;
    BOOST_LOG_TRIVIAL(debug) << "IsJumping: " << mIsJumping;
    motionComponent->SetVelocityOneAxis(Axis::X, direction * MOVE_SPEED_PER_TICKS);
    if (aboutToJump) {
        BOOST_LOG_TRIVIAL(debug) << "Jump";
        mIsJumping = true;
        motionComponent->SetVelocityOneAxis(Axis::Y, -JUMP_INITIAL_SPEED);
        mWantToJump = false;
    }
    BOOST_LOG_TRIVIAL(debug) << "Step player";
    return Error::OK;
}

void simple_2d::PlatformPlayer::SetWantToJump(bool wantToJump) {
    mWantToJump = wantToJump;
}

void simple_2d::PlatformPlayer::SetIsMovingLeft(bool isMovingLeft) {
    mIsMovingLeft = isMovingLeft;
}

void simple_2d::PlatformPlayer::SetIsMovingRight(bool isMovingRight) {
    mIsMovingRight = isMovingRight;
}