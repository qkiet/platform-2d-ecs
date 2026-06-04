#include <simple-2d/scene.h>
#include <simple-2d/utils.h>
#include <simple-2d/components/behavior_script.h>
#include <simple-2d/components/downward_gravity.h>
#include <simple-2d/components/static_sprite.h>
#include <simple-2d/components/motion.h>
#include <simple-2d/components/animated_sprite.h>
#include <simple-2d/components/json.h>
#include <simple-2d/components/static_repetitive_sprite.h>
#include <simple-2d/components/collision_body.h>

simple_2d::Scene::Scene(RectangularDimensions<int> dimensions) : mDimensions(dimensions) {
}

simple_2d::RectangularDimensions<int> simple_2d::Scene::GetDimensions() const {
    return mDimensions;
}

simple_2d::Error simple_2d::Scene::Init() {
    if (mIsInitialized) {
        return Error::OK;
    }
    mIsInitialized = true;
    mComponentManagers[BEHAVIOR_SCRIPT] = std::make_shared<BehaviorScriptComponentManager>();
    mComponentManagers[DOWNWARD_GRAVITY] = std::make_shared<DownwardGravityComponentManager>();
    mComponentManagers[STATIC_SPRITE] = std::make_shared<StaticSpriteComponentManager>();
    mComponentManagers[MOTION] = std::make_shared<MotionComponentManager>();
    mComponentManagers[ANIMATED_SPITE] = std::make_shared<AnimatedSpriteComponentManager>();
    mComponentManagers[JSON] = std::make_shared<JsonComponentManager>();
    mComponentManagers[STATIC_REPETITIVE_SPRITE] = std::make_shared<StaticRepetitiveSpriteComponentManager>();
    mComponentManagers[COLLISION_BODY] = std::make_shared<CollisionBodyComponentManager>();
    return Error::OK;
}

std::shared_ptr<simple_2d::ComponentManager> simple_2d::Scene::GetComponentManager(ComponentType componentType) const {
    if ((componentType < BEGIN_COMPONENT_TYPE) || (componentType > MAX_COMPONENT_TYPES)) {
        SIMPLE_2D_LOG_ERROR << "Accessing wrong component type " << componentType;
        return nullptr;
    }
    return mComponentManagers[componentType];
}

simple_2d::Error simple_2d::Scene::Step() {
    mComponentManagers[BEHAVIOR_SCRIPT]->Step();
    mComponentManagers[DOWNWARD_GRAVITY]->Step();
    mComponentManagers[STATIC_SPRITE]->Step();
    mComponentManagers[COLLISION_BODY]->Step();
    mComponentManagers[MOTION]->Step();
    mComponentManagers[ANIMATED_SPITE]->Step();
    mComponentManagers[STATIC_REPETITIVE_SPRITE]->Step();
    SIMPLE_2D_LOG_DEBUG << "Stepping component managers done";
    // Delete queued entity Id
    // NOTE: I really don't know whether this is good idea for removing entity from scene. I choose
    // this solution because I think that removing it after step through all component manager is better idea
    // than remove enity inside step function
    for (auto &entityId: mEntityIdsToDelete) {
        // To remove entity is simple: just remove every component of that entity
        // RemoveEntity a non-existing component of entity is OK
        mComponentManagers[BEHAVIOR_SCRIPT]->RemoveComponentOfEntity(entityId);
        mComponentManagers[DOWNWARD_GRAVITY]->RemoveComponentOfEntity(entityId);
        mComponentManagers[STATIC_SPRITE]->RemoveComponentOfEntity(entityId);
        mComponentManagers[COLLISION_BODY]->RemoveComponentOfEntity(entityId);
        mComponentManagers[MOTION]->RemoveComponentOfEntity(entityId);
        mComponentManagers[ANIMATED_SPITE]->RemoveComponentOfEntity(entityId);
        mComponentManagers[STATIC_REPETITIVE_SPRITE]->RemoveComponentOfEntity(entityId);
    }
    mEntityIdsToDelete.clear();
    return Error::OK;
}


void simple_2d::Scene::RequestDeleteEntity(EntityId entityId) {
    SIMPLE_2D_LOG_DEBUG << "Request delete entity " << entityId;
    mEntityIdsToDelete.push_back(entityId);
}
