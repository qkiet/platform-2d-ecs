#ifndef SIMPLE_2D_COMPONENT_DOWNWARD_GRAVITY_H
#define SIMPLE_2D_COMPONENT_DOWNWARD_GRAVITY_H

#include <simple-2d/component.h>

namespace simple_2d {
    class DownwardGravity : public Component {
    public:
        DownwardGravity(EntityId entityId);
        ~DownwardGravity() = default;
        Error Step() override;
    };

    class DownwardGravityComponentManager : public ComponentManager {
    public:
        DownwardGravityComponentManager() = default;
        ~DownwardGravityComponentManager() = default;
        void Step() override;
    };
}

#endif // SIMPLE_2D_DOWNWARD_GRAVITY_H
