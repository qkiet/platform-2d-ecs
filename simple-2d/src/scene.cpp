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
    mComponentManagers["behavior_script"] = std::make_shared<BehaviorScriptComponentManager>();
    mComponentManagers["downward_gravity"] = std::make_shared<DownwardGravityComponentManager>();
    mComponentManagers["static_sprite"] = std::make_shared<StaticSpriteComponentManager>();
    mComponentManagers["motion"] = std::make_shared<MotionComponentManager>();
    mComponentManagers["animated_sprite"] = std::make_shared<AnimatedSpriteComponentManager>();
    mComponentManagers["json"] = std::make_shared<JsonComponentManager>();
    mComponentManagers["static_repetitive_sprite"] = std::make_shared<StaticRepetitiveSpriteComponentManager>();
    mComponentManagers["collision_body"] = std::make_shared<CollisionBodyComponentManager>();
    return Error::OK;
}

std::shared_ptr<simple_2d::ComponentManager> simple_2d::Scene::GetComponentManager(const std::string& component_name) const {
    auto it = mComponentManagers.find(component_name);
    if (it == mComponentManagers.end()) {
        SIMPLE_2D_LOG_ERROR << "Component manager not found: " << component_name;
        return nullptr;
    }
    return it->second;
}

simple_2d::Error simple_2d::Scene::Step() {
    SIMPLE_2D_LOG_DEBUG << "Stepping component manager behavior_script";
    mComponentManagers["behavior_script"]->Step();
    SIMPLE_2D_LOG_DEBUG << "Stepping component manager downward_gravity";
    mComponentManagers["downward_gravity"]->Step();
    SIMPLE_2D_LOG_DEBUG << "Stepping component manager static_sprite";
    mComponentManagers["static_sprite"]->Step();
    SIMPLE_2D_LOG_DEBUG << "Stepping component manager collision_body";
    mComponentManagers["collision_body"]->Step();
    SIMPLE_2D_LOG_DEBUG << "Stepping component manager motion";
    mComponentManagers["motion"]->Step();
    SIMPLE_2D_LOG_DEBUG << "Stepping component manager animated_sprite";
    mComponentManagers["animated_sprite"]->Step();
    SIMPLE_2D_LOG_DEBUG << "Stepping component manager static_repetitive_sprite";
    mComponentManagers["static_repetitive_sprite"]->Step();
    SIMPLE_2D_LOG_DEBUG << "Stepping component managers done";
    return Error::OK;
}
