#include "ground.h"
#include <simple-2d/core.h>
#include <simple-2d/components/static_repetitive_sprite.h>
#include <simple-2d/components/motion.h>
#include <simple-2d/components/collision_body.h>
#include <simple-2d/components/json.h>
#include <boost/log/trivial.hpp>

simple_2d::Error Ground::Init() {
    auto &engine = simple_2d::Engine::GetInstance();
    auto error = AddComponent("static_repetitive_sprite");
    if (error != simple_2d::Error::OK) {
        BOOST_LOG_TRIVIAL(error) << "Failed to add static_repetitive_sprite component";
        return error;
    }
    error = AddComponent("motion");
    if (error != simple_2d::Error::OK) {
        BOOST_LOG_TRIVIAL(error) << "Failed to add motion component";
        return error;
    }
    error = AddComponent("collision_body");
    if (error != simple_2d::Error::OK) {
        BOOST_LOG_TRIVIAL(error) << "Failed to add collision_body component";
        return error;
    }
    error = AddComponent("json");
    if (error != simple_2d::Error::OK) {
        BOOST_LOG_TRIVIAL(error) << "Failed to add json component";
        return error;
    }
    auto jsonComponent = std::static_pointer_cast<simple_2d::JsonComponent>(GetComponent("json"));
    jsonComponent->SetJson(nlohmann::json::object({{"type", "ground"}}));
    auto groundBitmapBundle = engine.GetGraphics().LoadImageFromFile("assets/ground_tile.png");
    auto repetitiveSprite = std::static_pointer_cast<simple_2d::StaticRepetitiveSpriteComponent>(GetComponent("static_repetitive_sprite"));
    repetitiveSprite->SetUnitSurface(groundBitmapBundle.surface);
    repetitiveSprite->SetDimensions(simple_2d::RectangularDimensions<int>(512, 128));
    auto motion = std::static_pointer_cast<simple_2d::MotionComponent>(GetComponent("motion"));
    motion->SetPosition(simple_2d::XYCoordinate<float>(100, 400));
    auto collisionBody = std::static_pointer_cast<simple_2d::CollisionBodyComponent>(GetComponent("collision_body"));
    collisionBody->SetSize(simple_2d::RectangularDimensions<float>(512, 128));
    collisionBody->SetEnabled(true);
    collisionBody->SetOnCollisionCallback([](simple_2d::EntityId entityId1, simple_2d::EntityId entityId2) {
        BOOST_LOG_TRIVIAL(info) << "Ground collide with entity " << entityId2;
    });
    return simple_2d::Error::OK;
}

