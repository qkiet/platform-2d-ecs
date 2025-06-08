#ifndef SIMPLE_2D_COMPONENT_COLLISION_BODY_H
#define SIMPLE_2D_COMPONENT_COLLISION_BODY_H

#include <simple-2d/component.h>
#include <simple-2d/graphics.h>
#include <simple-2d/generic_types.h>
#include <set>

namespace simple_2d {
    class CollisionBodyComponent : public Component {
    public:
        struct CollisionResult {
            bool is_colliding = false;
            Direction direction;
        };

        // There are 4 types of collisions, provided that there is no collision before
        enum CollisionType {
            Cb1BottomEdgeCollidingWithCb2TopEdge,
            Cb1TopEdgeCollidingWithCb2BottomEdge,
            Cb1LeftEdgeCollidingWithCb2RightEdge,
            Cb1RightEdgeCollidingWithCb2LeftEdge,
        };

        typedef std::function<void(EntityId, EntityId, CollisionType)> OnCollisionCallback;
        CollisionBodyComponent(EntityId entityId);
        ~CollisionBodyComponent() = default;
        void SetEnabled(bool enabled);
        bool IsEnabled() const;
        void SetSize(RectangularDimensions<float> size);
        RectangularDimensions<float> GetSize() const;
        void SetOffset(XYCoordinate<float> offset);
        XYCoordinate<float> GetOffset() const;
        std::pair<Error, Rectangle<float>> GetCollisionBox() const;
        std::pair<Error, Rectangle<float>> GetCollisionBoxNextTick() const;
        void SetOnCollisionCallback(OnCollisionCallback callback);
        void NotifyCollision(EntityId otherEntityId, CollisionType collisionType);
        Error Step() override;
    private:
        bool mIsEnabled = true;
        RectangularDimensions<float> mSize;
        XYCoordinate<float> mOffset;
        CollisionResult mCollisionResult;
        // The callback will be called when 2 entities collide. First entity is always the entity that register callback
        OnCollisionCallback mOnCollisionCallback;
    };

    class CollisionBodyComponentManager : public ComponentManager {
    public:
        // The entire scene of the game will be divided into a grid of square cells. Each cell will have a unique id.
        typedef uint32_t CollisionCellId;
        CollisionBodyComponentManager();
        ~CollisionBodyComponentManager() = default;
        void Step() override;
    private:
        // Each cell will have a list of unique entities that are in the cell.
        std::map<CollisionCellId, std::set<EntityId>> mCollisionCellEntitiesMap;
        uint32_t mNumCellsX;
        uint32_t mNumCellsY;
        CollisionCellId GetCollisionCellId(XYCoordinate<CollisionCellId> cellIdPosition);
    };
};

#endif // SIMPLE_2D_COMPONENT_COLLISION_BODY_H
