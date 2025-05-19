#ifndef PLAYER_H
#define PLAYER_H
#include <simple-2d/entity.h>
#include <simple-2d/error_type.h>

class Player : public simple_2d::Entity {
    public:
        Player() = default;
        ~Player() = default;
        simple_2d::Error Init();
    private:
        bool mIsMovingLeft = false;
        bool mIsMovingRight = false;
};

#endif
