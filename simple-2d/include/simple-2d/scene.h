#ifndef SIMPLE_2D_SCENE_H
#define SIMPLE_2D_SCENE_H

#include "geometry.h"
#include "component.h"

namespace simple_2d {
    class Scene {
    private:
        bool mIsInitialized = false;
        RectangularDimensions<int> mDimensions;
        std::shared_ptr<ComponentManager> mComponentManagers[MAX_COMPONENT_TYPES];
    public:
        Scene(RectangularDimensions<int> dimensions);
        ~Scene() = default;
        Error Init();
        RectangularDimensions<int> GetDimensions() const;
        std::shared_ptr<ComponentManager> GetComponentManager(ComponentType componentType) const;
        Error Step();
    };
}
#endif // SIMPLE_2D_SCENE_H
