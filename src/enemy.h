#ifndef ENEMY_H
#define ENEMY_H
#include <simple-2d/entity.h>
#include <simple-2d/error_type.h>

class Enemy : public simple_2d::Entity {
    public:
        Enemy() = default;
        ~Enemy() = default;
        simple_2d::Error Init();
};

#endif
