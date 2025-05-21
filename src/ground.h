#ifndef GROUND_H
#define GROUND_H
#include <simple-2d/entity.h>
#include <simple-2d/error_type.h>

class Ground : public simple_2d::Entity {
    public:
        Ground() = default;
        ~Ground() = default;
        simple_2d::Error Init();
    private:
        bool mIsMovingLeft = false;
        bool mIsMovingRight = false;
};

#endif
