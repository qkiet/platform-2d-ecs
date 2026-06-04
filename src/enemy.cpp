#include "enemy.h"
#include <simple-2d/core.h>
#include <simple-2d/utils.h>
#include <simple-2d/components/animated_sprite.h>
#include <simple-2d/components/motion.h>
#include <simple-2d/components/behavior_script.h>
#include <simple-2d/components/json.h>
#include <simple-2d/components/collision_body.h>

#define MOVE_SPEED_PER_TICKS 2
#define JUMP_INITIAL_SPEED_WHEN_PLAYER_HIT_HEAD 6


simple_2d::Error Enemy::Init() {
    auto &engine = simple_2d::Engine::GetInstance();
    auto error = AddComponent(simple_2d::ComponentType::ANIMATED_SPITE);
    if (error != simple_2d::Error::OK) {
        SIMPLE_2D_LOG_ERROR << "Failed to add animated_sprite component";
        return error;
    }
    error = AddComponent(simple_2d::ComponentType::MOTION);
    if (error != simple_2d::Error::OK) {
        SIMPLE_2D_LOG_ERROR << "Failed to add motion component";
        return error;
    }
    error = AddComponent(simple_2d::ComponentType::DOWNWARD_GRAVITY);
    if (error != simple_2d::Error::OK) {
        SIMPLE_2D_LOG_ERROR << "Failed to add downward_gravity component";
        return error;
    }
    error = AddComponent(simple_2d::ComponentType::JSON);
    if (error != simple_2d::Error::OK) {
        SIMPLE_2D_LOG_ERROR << "Failed to add json component";
        return error;
    }
    error = AddComponent(simple_2d::ComponentType::COLLISION_BODY);
    if (error != simple_2d::Error::OK) {
        SIMPLE_2D_LOG_ERROR << "Failed to add collision_body component";
        return error;
    }
    error = AddComponent(simple_2d::ComponentType::BEHAVIOR_SCRIPT);
    if (error != simple_2d::Error::OK) {
        SIMPLE_2D_LOG_ERROR << "Failed to add behaviro_script component";
        return error;
    }
    auto animatedSprite = std::static_pointer_cast<simple_2d::AnimatedSprite>(GetComponent(simple_2d::ComponentType::ANIMATED_SPITE));
    auto bitmap = engine.GetGraphics().LoadImageFromFile("assets/player_idle_1.png");
    animatedSprite->AddAnimation(0, bitmap.texture, 5);
    bitmap = engine.GetGraphics().LoadImageFromFile("assets/player_idle_2.png");
    animatedSprite->AddAnimation(0, bitmap.texture, 5);
    bitmap = engine.GetGraphics().LoadImageFromFile("assets/player_idle_3.png");
    animatedSprite->AddAnimation(0, bitmap.texture, 5);
    animatedSprite->PlayAnimation(0);
    auto motion = std::static_pointer_cast<simple_2d::MotionComponent>(GetComponent(simple_2d::ComponentType::MOTION));
    motion->SetPosition(simple_2d::XYCoordinate<float>(600, 200));
    motion->SetVelocityOneAxis(simple_2d::Axis::X, -MOVE_SPEED_PER_TICKS);
    auto json = std::static_pointer_cast<simple_2d::JsonComponent>(GetComponent(simple_2d::ComponentType::JSON));
    json->SetJson(nlohmann::json::parse(R"(
        {
            "type": "enemy",
            "is_dead": true
        }
    )"));
    auto collisionBody = std::static_pointer_cast<simple_2d::CollisionBodyComponent>(GetComponent(simple_2d::ComponentType::COLLISION_BODY));
    // @TODO: Very hard-cody. Will use another method to get size of sprite then apply to collision body
    collisionBody->SetSize(simple_2d::RectangularDimensions<float>(60, 112));
    collisionBody->SetEnabled(true);
    collisionBody->SetOnCollisionCallback([](simple_2d::EntityId entityId1, simple_2d::EntityId entityId2, simple_2d::CollisionBodyComponent::CollisionType collisionType) {
        SIMPLE_2D_LOG_INFO << "Enemy collide with entity " << entityId2;
        auto &engine = simple_2d::Engine::GetInstance();
        auto jsonComponent1 = std::static_pointer_cast<simple_2d::JsonComponent>(engine.GetComponentManager(simple_2d::ComponentType::JSON)->GetComponent(entityId1));
        auto jsonComponent2 = std::static_pointer_cast<simple_2d::JsonComponent>(engine.GetComponentManager(simple_2d::ComponentType::JSON)->GetComponent(entityId2));
        auto jsonData1 = jsonComponent1->GetJson();
        auto jsonData2 = jsonComponent2->GetJson();
        if (jsonData2.contains("type") && jsonData2["type"] == "player") {
            SIMPLE_2D_LOG_INFO << "Enemy hit player";
            if (collisionType != simple_2d::CollisionBodyComponent::CollisionType::Cb1TopEdgeCollidingWithCb2BottomEdge) {
                // simple this is the case that player will die
                return;
            }
            auto motionComponent1 = std::static_pointer_cast<simple_2d::MotionComponent>(engine.GetComponentManager(simple_2d::ComponentType::MOTION)->GetComponent(entityId1));
            auto collisionBody1 = std::static_pointer_cast<simple_2d::CollisionBodyComponent>(engine.GetComponentManager(simple_2d::ComponentType::COLLISION_BODY)->GetComponent(entityId1));
            collisionBody1->SetEnabled(false);
            motionComponent1->SetVelocityOneAxis(simple_2d::Axis::Y, -JUMP_INITIAL_SPEED_WHEN_PLAYER_HIT_HEAD);
            SIMPLE_2D_LOG_INFO << "Enemy is dead";
            jsonData1["is_dead"] = true;
        }
    });
    // This ontick callback is simple for cleaning up enemy if they're dead and fall to certain distance
    auto behaviorScript = std::static_pointer_cast<simple_2d::BehaviorScript>(GetComponent(simple_2d::ComponentType::BEHAVIOR_SCRIPT));
    behaviorScript->SetOnTickEventCallback([](simple_2d::EntityId entityId){\
        auto &engine = simple_2d::Engine::GetInstance();
        auto jsonComponent = std::static_pointer_cast<simple_2d::JsonComponent>(engine.GetComponentManager(simple_2d::ComponentType::JSON)->GetComponent(entityId));
        auto jsonData = jsonComponent->GetJson();
        if (!jsonData["is_dead"]) {
            return;
        }
        auto motionComponent = std::static_pointer_cast<simple_2d::MotionComponent>(engine.GetComponentManager(simple_2d::ComponentType::MOTION)->GetComponent(entityId));
        auto posY = motionComponent->GetPositionOneAxis(simple_2d::Axis::Y);
        SIMPLE_2D_LOG_DEBUG << "Position of enemy is " << posY;
        if (posY > 500) {
            auto scene = engine.GetCurrentScene();
            SIMPLE_2D_LOG_DEBUG << "Enemy is requested to be deleted now!";
            scene->RequestDeleteEntity(entityId);
        }

    });
    return simple_2d::Error::OK;
}


