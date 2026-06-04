#include "ground.h"
#include <simple-2d/core.h>
#include <simple-2d/components/static_repetitive_sprite.h>
#include <simple-2d/components/motion.h>
#include <simple-2d/components/collision_body.h>
#include <simple-2d/components/json.h>
#include <simple-2d/utils.h>

simple_2d::Error Ground::Init() {
    auto &engine = simple_2d::Engine::GetInstance();
    auto error = AddComponent(simple_2d::ComponentType::STATIC_REPETITIVE_SPRITE);
    if (error != simple_2d::Error::OK) {
        SIMPLE_2D_LOG_ERROR << "Failed to add static_repetitive_sprite component";
        return error;
    }
    error = AddComponent(simple_2d::ComponentType::MOTION);
    if (error != simple_2d::Error::OK) {
        SIMPLE_2D_LOG_ERROR << "Failed to add motion component";
        return error;
    }
    error = AddComponent(simple_2d::ComponentType::COLLISION_BODY);
    if (error != simple_2d::Error::OK) {
        SIMPLE_2D_LOG_ERROR << "Failed to add collision_body component";
        return error;
    }
    error = AddComponent(simple_2d::ComponentType::JSON);
    if (error != simple_2d::Error::OK) {
        SIMPLE_2D_LOG_ERROR << "Failed to add json component";
        return error;
    }
    auto jsonComponent = std::static_pointer_cast<simple_2d::JsonComponent>(GetComponent(simple_2d::ComponentType::JSON));
    jsonComponent->SetJson(nlohmann::json::object({{"type", "ground"}}));
    auto groundBitmapBundle = engine.GetGraphics().LoadImageFromFile("assets/ground_tile.png");
    auto repetitiveSprite = std::static_pointer_cast<simple_2d::StaticRepetitiveSpriteComponent>(GetComponent(simple_2d::ComponentType::STATIC_REPETITIVE_SPRITE));
    repetitiveSprite->SetUnitSurface(groundBitmapBundle.surface);
    repetitiveSprite->SetDimensions(simple_2d::RectangularDimensions<int>(1024, 128));
    auto motion = std::static_pointer_cast<simple_2d::MotionComponent>(GetComponent(simple_2d::ComponentType::MOTION));
    motion->SetPosition(simple_2d::XYCoordinate<float>(100, 400));
    auto collisionBody = std::static_pointer_cast<simple_2d::CollisionBodyComponent>(GetComponent(simple_2d::ComponentType::COLLISION_BODY));
    collisionBody->SetSize(simple_2d::RectangularDimensions<float>(1024, 128));
    collisionBody->SetEnabled(true);
    return simple_2d::Error::OK;
}

