#include <simple-2d/components/collision_body.h>
#include <simple-2d/core.h>
#include <simple-2d/components/motion.h>
#include <boost/log/trivial.hpp>
#include <simple-2d/components/config.h>
#include <cmath>

// There are 4 types of collisions, provided that there is no collision before
enum CollisionType {
    Cb1BottomEdgeCollidingWithCb2TopEdge,
    Cb1TopEdgeCollidingWithCb2BottomEdge,
    Cb1LeftEdgeCollidingWithCb2RightEdge,
    Cb1RightEdgeCollidingWithCb2LeftEdge,
};

simple_2d::CollisionBodyComponent::CollisionBodyComponent(EntityId entityId) {
    mEntityId = entityId;
}


void simple_2d::CollisionBodyComponent::SetEnabled(bool enabled) {
    mIsEnabled = enabled;
}

bool simple_2d::CollisionBodyComponent::IsEnabled() const {
    return mIsEnabled;
}

void simple_2d::CollisionBodyComponent::SetSize(RectangularDimensions<float> size) {
    mSize = size;
}

simple_2d::RectangularDimensions<float> simple_2d::CollisionBodyComponent::GetSize() const {
    return mSize;
}

void simple_2d::CollisionBodyComponent::SetOffset(XYCoordinate<float> offset) {
    mOffset = offset;
}

simple_2d::XYCoordinate<float> simple_2d::CollisionBodyComponent::GetOffset() const {
    return mOffset;
}

std::pair<simple_2d::Error, simple_2d::Rectangle<float>> simple_2d::CollisionBodyComponent::GetCollisionBox() const {
    auto motionComponentManager = Engine::GetInstance().GetComponentManager("motion");
    if (motionComponentManager == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "Failed to get motion component manager";
        return {Error::NOT_EXISTS, Rectangle<float>()};
    }
    auto motionComponent = std::static_pointer_cast<MotionComponent>(motionComponentManager->GetComponent(GetEntityId()));
    if (motionComponent == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "Failed to get motion component";
        return {Error::NOT_EXISTS, Rectangle<float>()};
    }
    auto collisionBoxTopLeft = motionComponent->GetPosition() + mOffset;
    auto collisionBoxBottomRight = collisionBoxTopLeft + (XYCoordinate<float>)mSize;
    auto collisionBox = Rectangle<float>({collisionBoxTopLeft, collisionBoxBottomRight});
    return {Error::OK, collisionBox};
}

std::pair<simple_2d::Error, simple_2d::Rectangle<float>> simple_2d::CollisionBodyComponent::GetCollisionBoxNextTick() const {
    auto motionComponentManager = Engine::GetInstance().GetComponentManager("motion");
    if (motionComponentManager == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "Failed to get motion component manager";
        return {Error::NOT_EXISTS, Rectangle<float>()};
    }
    auto motionComponent = std::static_pointer_cast<MotionComponent>(motionComponentManager->GetComponent(GetEntityId()));
    if (motionComponent == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "Failed to get motion component";
        return {Error::NOT_EXISTS, Rectangle<float>()};
    }
    auto collisionBoxTopLeft = motionComponent->GetPositionNextTick() + mOffset;
    auto collisionBoxBottomRight = collisionBoxTopLeft + (XYCoordinate<float>)mSize;
    auto collisionBox = Rectangle<float>({collisionBoxTopLeft, collisionBoxBottomRight});
    return {Error::OK, collisionBox};
}

void simple_2d::CollisionBodyComponent::SetOnCollisionCallback(std::function<void(EntityId, EntityId)> callback) {
    mOnCollisionCallback = callback;
}

void simple_2d::CollisionBodyComponent::NotifyCollision(EntityId otherEntityId) {
    if (mOnCollisionCallback) {
        mOnCollisionCallback(GetEntityId(), otherEntityId);
    }
}

simple_2d::Error simple_2d::CollisionBodyComponent::Step() {
    // Do nothing since callback handles collision
    return Error::OK;
}

simple_2d::CollisionBodyComponentManager::CollisionBodyComponentManager() {
    // Because component manager are part of the scene, we can guarantee that current scene is not null
    auto sceneDimensions = Engine::GetInstance().GetCurrentScene()->GetDimensions();
    // Calculate the number of cells in the x directions
    // If the scene dimensions are not a multiple of CELL_SIZE, then the last cell will be covered partially
    if (sceneDimensions.width % CELL_SIZE == 0) {
        mNumCellsX = sceneDimensions.width / CELL_SIZE;
    } else {
        mNumCellsX = sceneDimensions.width / CELL_SIZE + 1;
    }
}

simple_2d::CollisionBodyComponentManager::CollisionCellId simple_2d::CollisionBodyComponentManager::GetCollisionCellId(XYCoordinate<CollisionCellId> cellIdPosition) {
    // The cell id is the index of the cell in the grid, which top left cell is 0,
    // below top left cell is mNumCellsX and below it is mNumCellsX * 2, etc.
    return cellIdPosition.y * mNumCellsX + cellIdPosition.x;
}


void simple_2d::CollisionBodyComponentManager::Step() {
    // Update the collision cell entities map
    mCollisionCellEntitiesMap.clear();
    for (auto &component : mComponents) {
        auto collisionBodyComponent = std::static_pointer_cast<CollisionBodyComponent>(component.second);
        if (!collisionBodyComponent->IsEnabled()) {
            BOOST_LOG_TRIVIAL(debug) << "Collision body component is not enabled for entity " << collisionBodyComponent->GetEntityId();
            continue;
        }
        auto motionComponentManager = Engine::GetInstance().GetComponentManager("motion");
        if (motionComponentManager == nullptr) {
            BOOST_LOG_TRIVIAL(error) << "Failed to get motion component manager";
            continue;
        }
        auto motionComponent = std::static_pointer_cast<MotionComponent>(motionComponentManager->GetComponent(collisionBodyComponent->GetEntityId()));
        if (motionComponent == nullptr) {
            BOOST_LOG_TRIVIAL(error) << "Failed to get motion component for entity " << collisionBodyComponent->GetEntityId();
            continue;
        }
        auto actualCollisionComponentOrigin = motionComponent->GetPosition() + collisionBodyComponent->GetOffset();
        auto collisionComponentSize = collisionBodyComponent->GetSize();
        auto collisionComponentTopLeft = actualCollisionComponentOrigin;
        auto collisionComponentTopRight = actualCollisionComponentOrigin + XYCoordinate<float>(collisionComponentSize.width, 0);
        auto collisionComponentBottomLeft = actualCollisionComponentOrigin + XYCoordinate<float>(0, collisionComponentSize.height);
        auto collisionComponentBottomRight = actualCollisionComponentOrigin + (XYCoordinate<float>)collisionComponentSize;
        XYCoordinate<CollisionCellId> topLeftCellId = {
            (CollisionCellId)(collisionComponentTopLeft.x / CELL_SIZE) + ((uint32_t)collisionComponentTopLeft.x % CELL_SIZE > 0),
            (CollisionCellId)(collisionComponentTopLeft.y / CELL_SIZE) + ((uint32_t)collisionComponentTopLeft.y % CELL_SIZE > 0)
        };
        XYCoordinate<CollisionCellId> topRightCellId = {
            (CollisionCellId)(collisionComponentTopRight.x / CELL_SIZE) + ((uint32_t)collisionComponentTopRight.x % CELL_SIZE > 0),
            (CollisionCellId)(collisionComponentTopRight.y / CELL_SIZE) + ((uint32_t)collisionComponentTopRight.y % CELL_SIZE > 0)
        };
        XYCoordinate<CollisionCellId> bottomLeftCellId = {
            (CollisionCellId)(collisionComponentBottomLeft.x / CELL_SIZE) + ((uint32_t)collisionComponentBottomLeft.x % CELL_SIZE > 0),
            (CollisionCellId)(collisionComponentBottomLeft.y / CELL_SIZE) + ((uint32_t)collisionComponentBottomLeft.y % CELL_SIZE > 0)
        };
        XYCoordinate<CollisionCellId> bottomRightCellId = {
            (CollisionCellId)(collisionComponentBottomRight.x / CELL_SIZE) + ((uint32_t)collisionComponentBottomRight.x % CELL_SIZE > 0),
            (CollisionCellId)(collisionComponentBottomRight.y / CELL_SIZE) + ((uint32_t)collisionComponentBottomRight.y % CELL_SIZE > 0)
        };
        // Add the collision component to the cells
        auto entityId = collisionBodyComponent->GetEntityId();
        auto generatedCellIds = [this](XYCoordinate<CollisionCellId> topLeftCellId, XYCoordinate<CollisionCellId> topRightCellId, XYCoordinate<CollisionCellId> bottomLeftCellId, XYCoordinate<CollisionCellId> bottomRightCellId) -> std::vector<CollisionCellId> {
            std::vector<CollisionCellId> res;
            for (auto y=topLeftCellId.y; y<=bottomRightCellId.y; y++) {
                for (auto x=topLeftCellId.x; x<=bottomRightCellId.x; x++) {
                    res.push_back(CollisionCellId(y * mNumCellsX + x));
                }
            }
            return res;
        };
        auto cellIds = generatedCellIds(topLeftCellId, topRightCellId, bottomLeftCellId, bottomRightCellId);
        for (auto cellId : cellIds) {
            BOOST_LOG_TRIVIAL(debug) << "Adding entity " << entityId << " to cell " << cellId;
            mCollisionCellEntitiesMap[cellId].insert(entityId);
        }
    }
    // Then check for collisions between entities in the same cell
    for (auto [_, entityIds]: mCollisionCellEntitiesMap) {
        for (auto it=entityIds.begin(); it!=entityIds.end(); it++) {
            for (auto it2=std::next(it); it2!=entityIds.end(); it2++) {
                auto entityId1 = *it;
                auto entityId2 = *it2;
                BOOST_LOG_TRIVIAL(debug) << "Checking collision between entities " << entityId1 << " and " << entityId2;
                auto collisionBodyComponent1 = std::static_pointer_cast<CollisionBodyComponent>(GetComponent(entityId1));
                if (nullptr == collisionBodyComponent1) {
                    BOOST_LOG_TRIVIAL(error) << "Failed to get collision body component for entity " << entityId1;
                    continue;
                }
                auto collisionBodyComponent2 = std::static_pointer_cast<CollisionBodyComponent>(GetComponent(entityId2));
                if (nullptr == collisionBodyComponent2) {
                    BOOST_LOG_TRIVIAL(error) << "Failed to get collision body component for entity " << entityId2;
                    continue;
                }
                // The algorithm is simple:
                // 1. Get the collision box of the 2 components next tick
                // 2. If the collision box of the 2 components next tick is not overlapping, then the 2 components are not colliding
                // 3. If the collision box of the 2 components next tick is overlapping, then the 2 components are colliding
                // 4. If the 2 components are colliding, then we need to figure out how to handle the motion of the 2 components based
                // on the direction of the collision by checking collision box before and after the collision.
                // However, for this to work properly, we need to assume that last tick, 2 collision components are not colliding!!
                // Later code will show this assumption
                auto [err1, collisionBoxThisTick1] = collisionBodyComponent1->GetCollisionBox();
                auto [err2, collisionBoxThisTick2] = collisionBodyComponent2->GetCollisionBox();
                if (Error::OK != err1 || Error::OK != err2) {
                    BOOST_LOG_TRIVIAL(error) << "Failed to get collision box for entity " << entityId1 << " or " << entityId2;
                    continue;
                }
                if (AreRectanglesOverlap(collisionBoxThisTick1, collisionBoxThisTick2)) {
                    BOOST_LOG_TRIVIAL(warning) << "Expected that 2 components are not colliding this tick, but they are";
                    // Currently, we don't know how to handle this case. So we just...let it stay overlapping.
                    // This is not a good solution, but it's the best we can do for now.
                    // TODO: We need to figure out how to handle this case.
                    continue;
                }
                auto [err3, collisionBoxNextTick1] = collisionBodyComponent1->GetCollisionBoxNextTick();
                auto [err4, collisionBoxNextTick2] = collisionBodyComponent2->GetCollisionBoxNextTick();
                if (Error::OK != err3 || Error::OK != err4) {
                    BOOST_LOG_TRIVIAL(error) << "Failed to get collision box next tick for entity " << entityId1 << " or " << entityId2;
                    continue;
                }
                if (!AreRectanglesOverlap(collisionBoxNextTick1, collisionBoxNextTick2)) {
                    continue;
                }
                auto cbThisTick1TopEdge = collisionBoxThisTick1.GetAxisAlignedEdge<float>(RectangleEdge::Top);
                auto cbThisTick1BottomEdge = collisionBoxThisTick1.GetAxisAlignedEdge<float>(RectangleEdge::Bottom);
                auto cbThisTick1LeftEdge = collisionBoxThisTick1.GetAxisAlignedEdge<float>(RectangleEdge::Left);
                auto cbThisTick1RightEdge = collisionBoxThisTick1.GetAxisAlignedEdge<float>(RectangleEdge::Right);
                auto cbThisTick2TopEdge = collisionBoxThisTick2.GetAxisAlignedEdge<float>(RectangleEdge::Top);
                auto cbThisTick2BottomEdge = collisionBoxThisTick2.GetAxisAlignedEdge<float>(RectangleEdge::Bottom);
                auto cbThisTick2LeftEdge = collisionBoxThisTick2.GetAxisAlignedEdge<float>(RectangleEdge::Left);
                auto cbThisTick2RightEdge = collisionBoxThisTick2.GetAxisAlignedEdge<float>(RectangleEdge::Right);
                auto cbNextTick1TopEdge = collisionBoxNextTick1.GetAxisAlignedEdge<float>(RectangleEdge::Top);
                auto cbNextTick1BottomEdge = collisionBoxNextTick1.GetAxisAlignedEdge<float>(RectangleEdge::Bottom);
                auto cbNextTick1LeftEdge = collisionBoxNextTick1.GetAxisAlignedEdge<float>(RectangleEdge::Left);
                auto cbNextTick1RightEdge = collisionBoxNextTick1.GetAxisAlignedEdge<float>(RectangleEdge::Right);
                auto cbNextTick2TopEdge = collisionBoxNextTick2.GetAxisAlignedEdge<float>(RectangleEdge::Top);
                auto cbNextTick2BottomEdge = collisionBoxNextTick2.GetAxisAlignedEdge<float>(RectangleEdge::Bottom);
                auto cbNextTick2LeftEdge = collisionBoxNextTick2.GetAxisAlignedEdge<float>(RectangleEdge::Left);
                auto cbNextTick2RightEdge = collisionBoxNextTick2.GetAxisAlignedEdge<float>(RectangleEdge::Right);
                // Magic happens here.
                // Check whether the bottom edge of cb1 is colliding with the top edge of cb2
                auto possibleCb1BottomEdgeCollidingWithCb2TopEdge = false;
                BOOST_LOG_TRIVIAL(debug) << "cbThisTick1BottomEdge: " << cbThisTick1BottomEdge;
                BOOST_LOG_TRIVIAL(debug) << "cbThisTick2TopEdge: " << cbThisTick2TopEdge;
                BOOST_LOG_TRIVIAL(debug) << "cbNextTick1BottomEdge: " << cbNextTick1BottomEdge;
                BOOST_LOG_TRIVIAL(debug) << "cbNextTick2TopEdge: " << cbNextTick2TopEdge;
                BOOST_LOG_TRIVIAL(debug) << "RelativePositionBetweenAxisAlignedEdges(cbThisTick1BottomEdge, cbThisTick2TopEdge): " << RelativePositionBetweenAxisAlignedEdges(cbThisTick1BottomEdge, cbThisTick2TopEdge);
                BOOST_LOG_TRIVIAL(debug) << "RelativePositionBetweenAxisAlignedEdges(cbNextTick1BottomEdge, cbNextTick2TopEdge): " << RelativePositionBetweenAxisAlignedEdges(cbNextTick1BottomEdge, cbNextTick2TopEdge);
                auto isCb1BottomEdgeAboveCb2TopEdgeThisTick = RelativePositionBetweenAxisAlignedEdges(cbThisTick1BottomEdge, cbThisTick2TopEdge) == AxisAlignedEdgesRelativePosition::Above;
                auto isCb1BottomEdgeBelowCb2TopEdgeNextTick = RelativePositionBetweenAxisAlignedEdges(cbNextTick1BottomEdge, cbNextTick2TopEdge) == AxisAlignedEdgesRelativePosition::Below;
                if (isCb1BottomEdgeAboveCb2TopEdgeThisTick && isCb1BottomEdgeBelowCb2TopEdgeNextTick) {
                    possibleCb1BottomEdgeCollidingWithCb2TopEdge = true;
                }
                auto possibleCb1TopEdgeCollidingWithCb2BottomEdge = false;
                BOOST_LOG_TRIVIAL(debug) << "cbThisTick1TopEdge: " << cbThisTick1TopEdge;
                BOOST_LOG_TRIVIAL(debug) << "cbThisTick2BottomEdge: " << cbThisTick2BottomEdge;
                BOOST_LOG_TRIVIAL(debug) << "cbNextTick1TopEdge: " << cbNextTick1TopEdge;
                BOOST_LOG_TRIVIAL(debug) << "cbNextTick2BottomEdge: " << cbNextTick2BottomEdge;
                BOOST_LOG_TRIVIAL(debug) << "RelativePositionBetweenAxisAlignedEdges(cbThisTick1TopEdge, cbThisTick2BottomEdge): " << RelativePositionBetweenAxisAlignedEdges(cbThisTick1TopEdge, cbThisTick2BottomEdge);
                BOOST_LOG_TRIVIAL(debug) << "RelativePositionBetweenAxisAlignedEdges(cbNextTick1TopEdge, cbNextTick2BottomEdge): " << RelativePositionBetweenAxisAlignedEdges(cbNextTick1TopEdge, cbNextTick2BottomEdge);
                auto isCb1TopEdgeBelowCb2BottomEdgeThisTick = RelativePositionBetweenAxisAlignedEdges(cbThisTick1TopEdge, cbThisTick2BottomEdge) == AxisAlignedEdgesRelativePosition::Below;
                auto isCb1TopEdgeAboveCb2BottomEdgeNextTick = RelativePositionBetweenAxisAlignedEdges(cbNextTick1TopEdge, cbNextTick2BottomEdge) == AxisAlignedEdgesRelativePosition::Above;
                if (isCb1TopEdgeBelowCb2BottomEdgeThisTick && isCb1TopEdgeAboveCb2BottomEdgeNextTick) {
                    possibleCb1TopEdgeCollidingWithCb2BottomEdge = true;
                }
                auto possibleCb1LeftEdgeCollidingWithCb2RightEdge = false;
                BOOST_LOG_TRIVIAL(debug) << "cbThisTick1LeftEdge: " << cbThisTick1LeftEdge;
                BOOST_LOG_TRIVIAL(debug) << "cbThisTick2RightEdge: " << cbThisTick2RightEdge;
                BOOST_LOG_TRIVIAL(debug) << "cbNextTick1LeftEdge: " << cbNextTick1LeftEdge;
                BOOST_LOG_TRIVIAL(debug) << "cbNextTick2RightEdge: " << cbNextTick2RightEdge;
                BOOST_LOG_TRIVIAL(debug) << "RelativePositionBetweenAxisAlignedEdges(cbThisTick1LeftEdge, cbThisTick2RightEdge): " << RelativePositionBetweenAxisAlignedEdges(cbThisTick1LeftEdge, cbThisTick2RightEdge);
                BOOST_LOG_TRIVIAL(debug) << "RelativePositionBetweenAxisAlignedEdges(cbNextTick1LeftEdge, cbNextTick2RightEdge): " << RelativePositionBetweenAxisAlignedEdges(cbNextTick1LeftEdge, cbNextTick2RightEdge);
                auto isCb1LeftEdgeRightOfCb2LeftEdgeThisTick = RelativePositionBetweenAxisAlignedEdges(cbThisTick1LeftEdge, cbThisTick2RightEdge) == AxisAlignedEdgesRelativePosition::RightOf;
                auto isCb1LeftEdgeLeftOfCb2RightEdgeNextTick = RelativePositionBetweenAxisAlignedEdges(cbNextTick1LeftEdge, cbNextTick2RightEdge) == AxisAlignedEdgesRelativePosition::LeftOf;
                if (isCb1LeftEdgeRightOfCb2LeftEdgeThisTick && isCb1LeftEdgeLeftOfCb2RightEdgeNextTick) {
                    possibleCb1LeftEdgeCollidingWithCb2RightEdge = true;
                }
                auto possibleCb1RightEdgeCollidingWithCb2LeftEdge = false;
                BOOST_LOG_TRIVIAL(debug) << "cbThisTick1RightEdge: " << cbThisTick1RightEdge;
                BOOST_LOG_TRIVIAL(debug) << "cbThisTick2LeftEdge: " << cbThisTick2LeftEdge;
                BOOST_LOG_TRIVIAL(debug) << "cbNextTick1RightEdge: " << cbNextTick1RightEdge;
                BOOST_LOG_TRIVIAL(debug) << "cbNextTick2LeftEdge: " << cbNextTick2LeftEdge;
                BOOST_LOG_TRIVIAL(debug) << "RelativePositionBetweenAxisAlignedEdges(cbThisTick1RightEdge, cbThisTick2LeftEdge): " << RelativePositionBetweenAxisAlignedEdges(cbThisTick1RightEdge, cbThisTick2LeftEdge);
                BOOST_LOG_TRIVIAL(debug) << "RelativePositionBetweenAxisAlignedEdges(cbNextTick1RightEdge, cbNextTick2LeftEdge): " << RelativePositionBetweenAxisAlignedEdges(cbNextTick1RightEdge, cbNextTick2LeftEdge);
                auto isCb1RightEdgeLeftOfCb2LeftEdgeThisTick = RelativePositionBetweenAxisAlignedEdges(cbThisTick1RightEdge, cbThisTick2LeftEdge) == AxisAlignedEdgesRelativePosition::LeftOf;
                auto isCb1RightEdgeRightOfCb2LeftEdgeNextTick = RelativePositionBetweenAxisAlignedEdges(cbNextTick1RightEdge, cbNextTick2LeftEdge) == AxisAlignedEdgesRelativePosition::RightOf;
                if (isCb1RightEdgeLeftOfCb2LeftEdgeThisTick && isCb1RightEdgeRightOfCb2LeftEdgeNextTick) {
                    possibleCb1RightEdgeCollidingWithCb2LeftEdge = true;
                }
                auto distanceCb1BottomEdgeToCb2TopEdgeNextTick = GetDistanceBetweenAxisAlignedEdges(cbNextTick1BottomEdge, cbNextTick2TopEdge);
                auto distanceCb1LeftEdgeToCb2RightEdgeNextTick = GetDistanceBetweenAxisAlignedEdges(cbNextTick1LeftEdge, cbNextTick2RightEdge);
                auto distanceCb1RightEdgeToCb2LeftEdgeNextTick = GetDistanceBetweenAxisAlignedEdges(cbNextTick1RightEdge, cbNextTick2LeftEdge);
                auto distanceCb1TopEdgeToCb2BottomEdgeNextTick = GetDistanceBetweenAxisAlignedEdges(cbNextTick1TopEdge, cbNextTick2BottomEdge);
                auto interpolateMotionForCollidingEntities = [this, distanceCb1BottomEdgeToCb2TopEdgeNextTick, distanceCb1LeftEdgeToCb2RightEdgeNextTick, distanceCb1RightEdgeToCb2LeftEdgeNextTick, distanceCb1TopEdgeToCb2BottomEdgeNextTick](EntityId entityId1, EntityId entityId2, CollisionType collisionType) {
                    BOOST_LOG_TRIVIAL(debug) << "Interpolating motion for entities " << entityId1 << " and " << entityId2 << " with collision type " << collisionType;
                    auto motionComponentManager = Engine::GetInstance().GetComponentManager("motion");
                    if (motionComponentManager == nullptr) {
                        BOOST_LOG_TRIVIAL(error) << "Failed to get motion component manager";
                        return;
                    }
                    auto motionComponent1 = std::static_pointer_cast<MotionComponent>(motionComponentManager->GetComponent(entityId1));
                    if (motionComponent1 == nullptr) {
                        BOOST_LOG_TRIVIAL(error) << "Failed to get motion component for entity " << entityId1;
                        return;
                    }
                    auto motionComponent2 = std::static_pointer_cast<MotionComponent>(motionComponentManager->GetComponent(entityId2));
                    if (motionComponent2 == nullptr) {
                        BOOST_LOG_TRIVIAL(error) << "Failed to get motion component for entity " << entityId2;
                        return;
                    }
                    auto collisionBodyComponent1 = std::static_pointer_cast<CollisionBodyComponent>(GetComponent(entityId1));
                    if (collisionBodyComponent1 == nullptr) {
                        BOOST_LOG_TRIVIAL(error) << "Failed to get collision body component for entity " << entityId1;
                        return;
                    }
                    auto collisionBodyComponent2 = std::static_pointer_cast<CollisionBodyComponent>(GetComponent(entityId2));
                    if (collisionBodyComponent2 == nullptr) {
                        BOOST_LOG_TRIVIAL(error) << "Failed to get collision body component for entity " << entityId2;
                        return;
                    }
                    auto [err1, collisionBoxThisTick1] = collisionBodyComponent1->GetCollisionBox();
                    if (Error::OK != err1) {
                        BOOST_LOG_TRIVIAL(error) << "Failed to get collision box next tick for entity " << entityId1 << " or " << entityId2;
                        return;
                    }
                    auto [err2, collisionBoxThisTick2] = collisionBodyComponent2->GetCollisionBox();
                    if (Error::OK != err2) {
                        BOOST_LOG_TRIVIAL(error) << "Failed to get collision box next tick for entity " << entityId1 << " or " << entityId2;
                        return;
                    }
                    auto [err3, collisionBoxNextTick1] = collisionBodyComponent1->GetCollisionBoxNextTick();
                    if (Error::OK != err1) {
                        BOOST_LOG_TRIVIAL(error) << "Failed to get collision box next tick for entity " << entityId1 << " or " << entityId2;
                        return;
                    }
                    auto [err4, collisionBoxNextTick2] = collisionBodyComponent2->GetCollisionBoxNextTick();
                    if (Error::OK != err2) {
                        BOOST_LOG_TRIVIAL(error) << "Failed to get collision box next tick for entity " << entityId1 << " or " << entityId2;
                        return;
                    }
                    auto topEdge1ThisTick = collisionBoxThisTick1.GetAxisAlignedEdge<float>(RectangleEdge::Top);
                    auto bottomEdge1ThisTick = collisionBoxThisTick1.GetAxisAlignedEdge<float>(RectangleEdge::Bottom);
                    auto leftEdge1ThisTick = collisionBoxThisTick1.GetAxisAlignedEdge<float>(RectangleEdge::Left);
                    auto rightEdge1ThisTick = collisionBoxThisTick1.GetAxisAlignedEdge<float>(RectangleEdge::Right);
                    auto topEdge2ThisTick = collisionBoxThisTick2.GetAxisAlignedEdge<float>(RectangleEdge::Top);
                    auto bottomEdge2ThisTick = collisionBoxThisTick2.GetAxisAlignedEdge<float>(RectangleEdge::Bottom);
                    auto leftEdge2ThisTick = collisionBoxThisTick2.GetAxisAlignedEdge<float>(RectangleEdge::Left);
                    auto rightEdge2ThisTick = collisionBoxThisTick2.GetAxisAlignedEdge<float>(RectangleEdge::Right);
                    auto topEdge1NextTick = collisionBoxNextTick1.GetAxisAlignedEdge<float>(RectangleEdge::Top);
                    auto bottomEdge1NextTick = collisionBoxNextTick1.GetAxisAlignedEdge<float>(RectangleEdge::Bottom);
                    auto leftEdge1NextTick = collisionBoxNextTick1.GetAxisAlignedEdge<float>(RectangleEdge::Left);
                    auto rightEdge1NextTick = collisionBoxNextTick1.GetAxisAlignedEdge<float>(RectangleEdge::Right);
                    auto topEdge2NextTick = collisionBoxNextTick2.GetAxisAlignedEdge<float>(RectangleEdge::Top);
                    auto bottomEdge2NextTick = collisionBoxNextTick2.GetAxisAlignedEdge<float>(RectangleEdge::Bottom);
                    auto leftEdge2NextTick = collisionBoxNextTick2.GetAxisAlignedEdge<float>(RectangleEdge::Left);
                    auto rightEdge2NextTick = collisionBoxNextTick2.GetAxisAlignedEdge<float>(RectangleEdge::Right);
                    auto velocityNextTick1 = motionComponent1->GetVelocityNextTick();
                    auto velocityNextTick2 = motionComponent2->GetVelocityNextTick();
                    auto velocityRatioY = 0.0f;
                    auto velocityRatioX = 0.0f;
                    // Resolve motions for 2 entities will be simple: we calculate the overlapped distance in the axis of the collision,
                    // and then we reallocate their positions based on ratio of velocities (acceleration added) in the axis of the collision.
                    switch (collisionType) {
                        case CollisionType::Cb1BottomEdgeCollidingWithCb2TopEdge:
                            velocityRatioY = velocityNextTick1.y / (velocityNextTick1.y + velocityNextTick2.y);
                            motionComponent1->SetPositionOneAxis(Axis::Y, motionComponent1->GetPositionOneAxis(Axis::Y) - distanceCb1BottomEdgeToCb2TopEdgeNextTick * velocityRatioY);
                            motionComponent2->SetPositionOneAxis(Axis::Y, motionComponent2->GetPositionOneAxis(Axis::Y) + distanceCb1BottomEdgeToCb2TopEdgeNextTick * (1 - velocityRatioY));
                            if (RelativePositionBetweenAxisAlignedEdges(bottomEdge1ThisTick, bottomEdge1NextTick) == AxisAlignedEdgesRelativePosition::Above) {
                                motionComponent1->SetVelocityOneAxis(Axis::Y, 0);
                                if (RelativePositionBetweenAxisAlignedEdges(topEdge2ThisTick, topEdge2NextTick) == AxisAlignedEdgesRelativePosition::Below) {
                                    motionComponent2->SetVelocityOneAxis(Axis::Y, 0);
                                }
                            }
                            break;
                        case CollisionType::Cb1TopEdgeCollidingWithCb2BottomEdge:
                            velocityRatioY = velocityNextTick1.y / (velocityNextTick1.y + velocityNextTick2.y);
                            motionComponent1->SetPositionOneAxis(Axis::Y, motionComponent1->GetPositionOneAxis(Axis::Y) + distanceCb1TopEdgeToCb2BottomEdgeNextTick * velocityRatioY);
                            motionComponent2->SetPositionOneAxis(Axis::Y, motionComponent2->GetPositionOneAxis(Axis::Y) - distanceCb1TopEdgeToCb2BottomEdgeNextTick * (1 - velocityRatioY));
                            if (RelativePositionBetweenAxisAlignedEdges(topEdge1ThisTick, topEdge1NextTick) == AxisAlignedEdgesRelativePosition::Below) {
                                motionComponent1->SetVelocityOneAxis(Axis::Y, 0);
                                if (RelativePositionBetweenAxisAlignedEdges(bottomEdge2ThisTick, bottomEdge2NextTick) == AxisAlignedEdgesRelativePosition::Above) {
                                    motionComponent2->SetVelocityOneAxis(Axis::Y, 0);
                                }
                            }
                            break;
                        case CollisionType::Cb1LeftEdgeCollidingWithCb2RightEdge:
                            velocityRatioX = velocityNextTick1.x / (velocityNextTick1.x + velocityNextTick2.x);
                            motionComponent1->SetPositionOneAxis(Axis::X, motionComponent1->GetPositionOneAxis(Axis::X) + distanceCb1LeftEdgeToCb2RightEdgeNextTick * velocityRatioX);
                            motionComponent2->SetPositionOneAxis(Axis::X, motionComponent2->GetPositionOneAxis(Axis::X) - distanceCb1LeftEdgeToCb2RightEdgeNextTick * (1 - velocityRatioX));
                            if (RelativePositionBetweenAxisAlignedEdges(leftEdge1ThisTick, leftEdge1NextTick) == AxisAlignedEdgesRelativePosition::RightOf) {
                                motionComponent1->SetVelocityOneAxis(Axis::X, 0);
                                if (RelativePositionBetweenAxisAlignedEdges(rightEdge2ThisTick, rightEdge2NextTick) == AxisAlignedEdgesRelativePosition::LeftOf) {
                                    motionComponent2->SetVelocityOneAxis(Axis::X, 0);
                                }
                            }
                            break;
                        case CollisionType::Cb1RightEdgeCollidingWithCb2LeftEdge:
                            velocityRatioX = velocityNextTick1.x / (velocityNextTick1.x + velocityNextTick2.x);
                            motionComponent1->SetPositionOneAxis(Axis::X, motionComponent1->GetPositionOneAxis(Axis::X) - distanceCb1RightEdgeToCb2LeftEdgeNextTick * velocityRatioX);
                            motionComponent2->SetPositionOneAxis(Axis::X, motionComponent2->GetPositionOneAxis(Axis::X) + distanceCb1RightEdgeToCb2LeftEdgeNextTick * (1 - velocityRatioX));
                            if (RelativePositionBetweenAxisAlignedEdges(rightEdge1ThisTick, rightEdge1NextTick) == AxisAlignedEdgesRelativePosition::LeftOf) {
                                motionComponent1->SetVelocityOneAxis(Axis::X, 0);
                                if (RelativePositionBetweenAxisAlignedEdges(leftEdge2ThisTick, leftEdge2NextTick) == AxisAlignedEdgesRelativePosition::RightOf) {
                                    motionComponent2->SetVelocityOneAxis(Axis::X, 0);
                                }
                            }
                            break;
                    }
                };
                BOOST_LOG_TRIVIAL(debug) << "Possible cb1 bottom edge colliding with cb2 top edge: " << possibleCb1BottomEdgeCollidingWithCb2TopEdge;
                BOOST_LOG_TRIVIAL(debug) << "Possible cb1 top edge colliding with cb2 bottom edge: " << possibleCb1TopEdgeCollidingWithCb2BottomEdge;
                BOOST_LOG_TRIVIAL(debug) << "Possible cb1 left edge colliding with cb2 right edge: " << possibleCb1LeftEdgeCollidingWithCb2RightEdge;
                BOOST_LOG_TRIVIAL(debug) << "Possible cb1 right edge colliding with cb2 left edge: " << possibleCb1RightEdgeCollidingWithCb2LeftEdge;
                if (possibleCb1BottomEdgeCollidingWithCb2TopEdge && possibleCb1LeftEdgeCollidingWithCb2RightEdge) {
                    // Another magic! Let me explain how to know what pair of edges will collide first:
                    // - We check the distance between 2 pairs of edges next tick.
                    // - How to decide what collide first? Which pair has smaller distance WILL BE COLLIDE FIRST. You can check by yourself
                    //   by a thought experiment: one rectangle is moving very fast in axis X but move slower in axis Y. If it
                    //   overlapped with another rectangle, axis X is definitely cover more distance than axis Y. But...it's
                    //   actually axis Y collide first!
                    if (distanceCb1BottomEdgeToCb2TopEdgeNextTick < distanceCb1LeftEdgeToCb2RightEdgeNextTick) {
                        // Bottom edge of cb1 is colliding with top edge of cb2.
                        interpolateMotionForCollidingEntities(entityId1, entityId2, CollisionType::Cb1BottomEdgeCollidingWithCb2TopEdge);
                    } else {
                        // The collision is happening in the X axis. Means left edge of cb1 is colliding with right edge of cb2.
                        interpolateMotionForCollidingEntities(entityId1, entityId2, CollisionType::Cb1LeftEdgeCollidingWithCb2RightEdge);
                    }
                } else if (possibleCb1BottomEdgeCollidingWithCb2TopEdge && possibleCb1RightEdgeCollidingWithCb2LeftEdge) {
                    if (distanceCb1BottomEdgeToCb2TopEdgeNextTick < distanceCb1RightEdgeToCb2LeftEdgeNextTick) {
                        interpolateMotionForCollidingEntities(entityId1, entityId2, CollisionType::Cb1BottomEdgeCollidingWithCb2TopEdge);
                    } else {
                        interpolateMotionForCollidingEntities(entityId1, entityId2, CollisionType::Cb1RightEdgeCollidingWithCb2LeftEdge);
                    }
                } else if (possibleCb1TopEdgeCollidingWithCb2BottomEdge && possibleCb1LeftEdgeCollidingWithCb2RightEdge) {
                    if (distanceCb1TopEdgeToCb2BottomEdgeNextTick < distanceCb1LeftEdgeToCb2RightEdgeNextTick) {
                        interpolateMotionForCollidingEntities(entityId1, entityId2, CollisionType::Cb1TopEdgeCollidingWithCb2BottomEdge);
                    } else {
                        interpolateMotionForCollidingEntities(entityId1, entityId2, CollisionType::Cb1LeftEdgeCollidingWithCb2RightEdge);
                    }
                } else if (possibleCb1TopEdgeCollidingWithCb2BottomEdge && possibleCb1RightEdgeCollidingWithCb2LeftEdge) {
                    if (distanceCb1TopEdgeToCb2BottomEdgeNextTick < distanceCb1RightEdgeToCb2LeftEdgeNextTick) {
                        interpolateMotionForCollidingEntities(entityId1, entityId2, CollisionType::Cb1TopEdgeCollidingWithCb2BottomEdge);
                    } else {
                        interpolateMotionForCollidingEntities(entityId1, entityId2, CollisionType::Cb1RightEdgeCollidingWithCb2LeftEdge);
                    }
                } else if (possibleCb1BottomEdgeCollidingWithCb2TopEdge) {
                    interpolateMotionForCollidingEntities(entityId1, entityId2, CollisionType::Cb1BottomEdgeCollidingWithCb2TopEdge);
                } else if (possibleCb1TopEdgeCollidingWithCb2BottomEdge) {
                    interpolateMotionForCollidingEntities(entityId1, entityId2, CollisionType::Cb1TopEdgeCollidingWithCb2BottomEdge);
                } else if (possibleCb1LeftEdgeCollidingWithCb2RightEdge) {
                    interpolateMotionForCollidingEntities(entityId1, entityId2, CollisionType::Cb1LeftEdgeCollidingWithCb2RightEdge);
                } else if (possibleCb1RightEdgeCollidingWithCb2LeftEdge) {
                    interpolateMotionForCollidingEntities(entityId1, entityId2, CollisionType::Cb1RightEdgeCollidingWithCb2LeftEdge);
                }
                // Because reach this point means 2 entities are colliding, so we can notify the collision without worrying about
                // whether the collision is happening or not.
                collisionBodyComponent1->NotifyCollision(entityId2);
                collisionBodyComponent2->NotifyCollision(entityId1);
            }
        }
    }
}

