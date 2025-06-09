#include "enemy.h"
#include <simple-2d/core.h>
#include <boost/log/trivial.hpp>
#include <simple-2d/components/animated_sprite.h>
#include <simple-2d/components/motion.h>
#include <simple-2d/components/behavior_script.h>
#include <simple-2d/components/json.h>
#include <simple-2d/components/collision_body.h>

#define MOVE_SPEED_PER_TICKS 2
#define JUMP_INITIAL_SPEED_WHEN_PLAYER_HIT_HEAD 6


simple_2d::Error Enemy::Init() {
    auto &engine = simple_2d::Engine::GetInstance();
    auto error = AddComponent("animated_sprite");
    if (error != simple_2d::Error::OK) {
        BOOST_LOG_TRIVIAL(error) << "Failed to add animated_sprite component";
        return error;
    }
    error = AddComponent("motion");
    if (error != simple_2d::Error::OK) {
        BOOST_LOG_TRIVIAL(error) << "Failed to add motion component";
        return error;
    }
    error = AddComponent("downward_gravity");
    if (error != simple_2d::Error::OK) {
        BOOST_LOG_TRIVIAL(error) << "Failed to add downward_gravity component";
        return error;
    }
    error = AddComponent("json");
    if (error != simple_2d::Error::OK) {
        BOOST_LOG_TRIVIAL(error) << "Failed to add json component";
        return error;
    }
    error = AddComponent("collision_body");
    if (error != simple_2d::Error::OK) {
        BOOST_LOG_TRIVIAL(error) << "Failed to add collision_body component";
        return error;
    }
    auto animatedSprite = std::static_pointer_cast<simple_2d::AnimatedSprite>(GetComponent("animated_sprite"));
    auto bitmap = engine.GetGraphics().LoadImageFromFile("assets/player_idle_1.png");
    animatedSprite->AddAnimation(0, bitmap.texture, 5);
    bitmap = engine.GetGraphics().LoadImageFromFile("assets/player_idle_2.png");
    animatedSprite->AddAnimation(0, bitmap.texture, 5);
    bitmap = engine.GetGraphics().LoadImageFromFile("assets/player_idle_3.png");
    animatedSprite->AddAnimation(0, bitmap.texture, 5);
    animatedSprite->PlayAnimation(0);
    auto motion = std::static_pointer_cast<simple_2d::MotionComponent>(GetComponent("motion"));
    motion->SetPosition(simple_2d::XYCoordinate<float>(600, 200));
    motion->SetVelocityOneAxis(simple_2d::Axis::X, -MOVE_SPEED_PER_TICKS);
    auto json = std::static_pointer_cast<simple_2d::JsonComponent>(GetComponent("json"));
    json->SetJson(nlohmann::json::parse(R"(
        {
            "type": "enemy"
        }
    )"));
    auto collisionBody = std::static_pointer_cast<simple_2d::CollisionBodyComponent>(GetComponent("collision_body"));
    // @TODO: Very hard-cody. Will use another method to get size of sprite then apply to collision body
    collisionBody->SetSize(simple_2d::RectangularDimensions<float>(60, 112));
    collisionBody->SetEnabled(true);
    collisionBody->SetOnCollisionCallback([](simple_2d::EntityId entityId1, simple_2d::EntityId entityId2, simple_2d::CollisionBodyComponent::CollisionType collisionType) {
        BOOST_LOG_TRIVIAL(info) << "Enemy collide with entity " << entityId2;
        auto &engine = simple_2d::Engine::GetInstance();
        auto jsonComponent1 = std::static_pointer_cast<simple_2d::JsonComponent>(engine.GetComponentManager("json")->GetComponent(entityId1));
        auto jsonComponent2 = std::static_pointer_cast<simple_2d::JsonComponent>(engine.GetComponentManager("json")->GetComponent(entityId2));
        auto jsonData1 = jsonComponent1->GetJson();
        auto jsonData2 = jsonComponent2->GetJson();
        if (jsonData2.contains("type") && jsonData2["type"] == "player") {
            BOOST_LOG_TRIVIAL(info) << "Enemy hit player";
            if (collisionType != simple_2d::CollisionBodyComponent::CollisionType::Cb1TopEdgeCollidingWithCb2BottomEdge) {
                // simple this is the case that player will die
                return;
            }
            auto motionComponent1 = std::static_pointer_cast<simple_2d::MotionComponent>(engine.GetComponentManager("motion")->GetComponent(entityId1));
            auto collisionBody1 = std::static_pointer_cast<simple_2d::CollisionBodyComponent>(engine.GetComponentManager("collision_body")->GetComponent(entityId1));
            collisionBody1->SetEnabled(false);
            motionComponent1->SetVelocityOneAxis(simple_2d::Axis::Y, -JUMP_INITIAL_SPEED_WHEN_PLAYER_HIT_HEAD);
            BOOST_LOG_TRIVIAL(info) << "Enemy is dead";
        }
    });
    return simple_2d::Error::OK;
}


