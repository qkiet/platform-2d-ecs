#include <simple-2d/scene.h>
#include <boost/log/trivial.hpp>

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
    std::vector<std::string> supportedComponentNames = {
        "behavior_script",
        "downward_gravity",
        "static_sprite",
        "motion",
        "animated_sprite",
        "json",
        "static_repetitive_sprite",
        "collision_body"
    };
    for (auto& component_name : supportedComponentNames) {
        mComponentManagers[component_name] = std::make_shared<ComponentManager>();
    }
    return Error::OK;
}

std::shared_ptr<simple_2d::ComponentManager> simple_2d::Scene::GetComponentManager(const std::string& component_name) const {
    auto it = mComponentManagers.find(component_name);
    if (it == mComponentManagers.end()) {
        BOOST_LOG_TRIVIAL(error) << "Component manager not found: " << component_name;
        return nullptr;
    }
    return it->second;
}

simple_2d::Error simple_2d::Scene::Step() {
    BOOST_LOG_TRIVIAL(debug) << "Stepping component manager behavior_script";
    mComponentManagers["behavior_script"]->Step();
    BOOST_LOG_TRIVIAL(debug) << "Stepping component manager downward_gravity";
    mComponentManagers["downward_gravity"]->Step();
    BOOST_LOG_TRIVIAL(debug) << "Stepping component manager static_sprite";
    mComponentManagers["static_sprite"]->Step();
    BOOST_LOG_TRIVIAL(debug) << "Stepping component manager motion";
    mComponentManagers["motion"]->Step();
    BOOST_LOG_TRIVIAL(debug) << "Stepping component manager animated_sprite";
    mComponentManagers["animated_sprite"]->Step();
    BOOST_LOG_TRIVIAL(debug) << "Stepping component manager static_repetitive_sprite";
    mComponentManagers["static_repetitive_sprite"]->Step();
    BOOST_LOG_TRIVIAL(debug) << "Stepping component managers done";
    return Error::OK;
}
