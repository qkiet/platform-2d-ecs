#ifndef SIMPLE_2D_PLATFORM_PLAYER_H
#define SIMPLE_2D_PLATFORM_PLAYER_H

#include <simple-2d/component.h>

namespace simple_2d {
    class PlatformPlayer : public Component {
    public:
        PlatformPlayer() = default;
        ~PlatformPlayer() = default;
        void RegisterEventsCallbacks();
        Error Step() override;
        void SetWantToJump(bool wantToJump);
        void SetIsMovingLeft(bool isMovingLeft);
        void SetIsMovingRight(bool isMovingRight);
        void SetIsJumping(bool isJumping);
    private:
        bool mWantToJump;
        bool mIsMovingLeft;
        bool mIsMovingRight;
        bool mIsJumping;
    };
}

#endif // SIMPLE_2D_PLATFORM_PLAYER_H
