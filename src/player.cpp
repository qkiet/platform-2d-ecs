#include "player.h"
#include <simple-2d/core.h>
#include <simple-2d/utils.h>
#include <simple-2d/components/animated_sprite.h>
#include <simple-2d/components/motion.h>
#include <simple-2d/components/behavior_script.h>
#include <simple-2d/components/json.h>
#include <simple-2d/components/collision_body.h>

#define MOVE_SPEED_PER_TICKS 3
#define JUMP_INITIAL_SPEED 10
#define JUMP_INITIAL_SPEED_WHEN_HIT_ENEMY 8

static void onKeyPressedEvent(simple_2d::EntityId entityId, const SDL_Event& event) {
    auto &engine = simple_2d::Engine::GetInstance();
    auto json = std::static_pointer_cast<simple_2d::JsonComponent>(engine.GetComponentManager("json")->GetComponent(entityId));
    auto jsonData = json->GetJson();
    SIMPLE_2D_LOG_DEBUG << "Key pressed event";
    if (event.key.scancode == SDL_SCANCODE_LEFT) {
        jsonData["isMovingLeft"] = true;
        SIMPLE_2D_LOG_DEBUG << "Left key pressed";
    } else if (event.key.scancode == SDL_SCANCODE_RIGHT) {
        jsonData["isMovingRight"] = true;
        SIMPLE_2D_LOG_DEBUG << "Right key pressed";
    }
    json->SetJson(jsonData);
}

static void onKeyReleasedEvent(simple_2d::EntityId entityId, const SDL_Event& event) {
    auto &engine = simple_2d::Engine::GetInstance();
    SIMPLE_2D_LOG_DEBUG << "Key released event";
    auto json = std::static_pointer_cast<simple_2d::JsonComponent>(engine.GetComponentManager("json")->GetComponent(entityId));
    auto jsonData = json->GetJson();
    if (event.key.scancode == SDL_SCANCODE_SPACE) {
        jsonData["wantToJump"] = true;
    } else if (event.key.scancode == SDL_SCANCODE_LEFT) {
        jsonData["isMovingLeft"] = false;
    } else if (event.key.scancode == SDL_SCANCODE_RIGHT) {
        jsonData["isMovingRight"] = false;
    }
    json->SetJson(jsonData);
}

static void onTickEvent(simple_2d::EntityId entityId) {
    auto &engine = simple_2d::Engine::GetInstance();
    auto json = std::static_pointer_cast<simple_2d::JsonComponent>(engine.GetComponentManager("json")->GetComponent(entityId));
    auto jsonData = json->GetJson();
    SIMPLE_2D_LOG_DEBUG << "Tick event";
    auto motionComponentManager = simple_2d::Engine::GetInstance().GetComponentManager("motion");
    if (motionComponentManager == nullptr) {
        SIMPLE_2D_LOG_ERROR << "Failed to get motion component manager";
        return;
    }
    auto component = motionComponentManager->GetComponent(entityId);
    if (component == nullptr) {
        SIMPLE_2D_LOG_ERROR << "Failed to get motion component for entity " << entityId;
        return;
    }
    auto motionComponent = std::static_pointer_cast<simple_2d::MotionComponent>(component);
    bool isMovingRight = jsonData["isMovingRight"];
    bool isMovingLeft = jsonData["isMovingLeft"];
    bool wantToJump = jsonData["wantToJump"];
    bool isJumping = jsonData["isJumping"];
    int direction = (int)isMovingRight - (int)isMovingLeft;
    SIMPLE_2D_LOG_DEBUG << "Direction: " << direction;
    int aboutToJump = wantToJump && !isJumping;
    SIMPLE_2D_LOG_DEBUG << "IsJumping: " << isJumping;
    motionComponent->SetVelocityOneAxis(simple_2d::Axis::X, direction * MOVE_SPEED_PER_TICKS);
    if (aboutToJump) {
        SIMPLE_2D_LOG_DEBUG << "Jump";
        motionComponent->SetVelocityOneAxis(simple_2d::Axis::Y, -JUMP_INITIAL_SPEED);
        jsonData["isJumping"] = true;
        jsonData["wantToJump"] = false;
    }
    json->SetJson(jsonData);
}

simple_2d::Error Player::Init() {
    auto &engine = simple_2d::Engine::GetInstance();
    auto error = AddComponent("animated_sprite");
    if (error != simple_2d::Error::OK) {
        SIMPLE_2D_LOG_ERROR << "Failed to add animated_sprite component";
        return error;
    }
    error = AddComponent("motion");
    if (error != simple_2d::Error::OK) {
        SIMPLE_2D_LOG_ERROR << "Failed to add motion component";
        return error;
    }
    error = AddComponent("behavior_script");
    if (error != simple_2d::Error::OK) {
        SIMPLE_2D_LOG_ERROR << "Failed to add behavior_script component";
        return error;
    }
    error = AddComponent("downward_gravity");
    if (error != simple_2d::Error::OK) {
        SIMPLE_2D_LOG_ERROR << "Failed to add downward_gravity component";
        return error;
    }
    error = AddComponent("json");
    if (error != simple_2d::Error::OK) {
        SIMPLE_2D_LOG_ERROR << "Failed to add json component";
        return error;
    }
    error = AddComponent("collision_body");
    if (error != simple_2d::Error::OK) {
        SIMPLE_2D_LOG_ERROR << "Failed to add collision_body component";
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
    motion->SetPosition(simple_2d::XYCoordinate<float>(200, 200));
    auto json = std::static_pointer_cast<simple_2d::JsonComponent>(GetComponent("json"));
    json->SetJson(nlohmann::json::parse(R"(
        {
            "isMovingLeft": false,
            "isMovingRight": false,
            "wantToJump": false,
            "isJumping": false,
            "type": "player"
        }
    )"));
    auto behaviorScript = std::static_pointer_cast<simple_2d::BehaviorScript>(GetComponent("behavior_script"));
    behaviorScript->SetOnKeyPressedEventCallback(onKeyPressedEvent);
    behaviorScript->SetOnKeyReleasedEventCallback(onKeyReleasedEvent);
    behaviorScript->SetOnTickEventCallback(onTickEvent);
    auto collisionBody = std::static_pointer_cast<simple_2d::CollisionBodyComponent>(GetComponent("collision_body"));
    // @TODO: Very hard-cody. Will use another method to get size of sprite then apply to collision body
    collisionBody->SetSize(simple_2d::RectangularDimensions<float>(60, 112));
    collisionBody->SetEnabled(true);
    collisionBody->SetOnCollisionCallback([](simple_2d::EntityId entityId1, simple_2d::EntityId entityId2, simple_2d::CollisionBodyComponent::CollisionType collisionType) {
        SIMPLE_2D_LOG_INFO << "Player collide with entity " << entityId2;
        auto &engine = simple_2d::Engine::GetInstance();
        auto jsonComponent1 = std::static_pointer_cast<simple_2d::JsonComponent>(engine.GetComponentManager("json")->GetComponent(entityId1));
        auto jsonComponent2 = std::static_pointer_cast<simple_2d::JsonComponent>(engine.GetComponentManager("json")->GetComponent(entityId2));
        if (jsonComponent2 == nullptr) {
            // Collide with uninteresting entity, do nothing
            return;
        }
        auto jsonData1 = jsonComponent1->GetJson();
        auto jsonData2 = jsonComponent2->GetJson();
        if (jsonData2.contains("type") && jsonData2["type"] == "ground") {
            jsonData1["isJumping"] = false;
            jsonComponent1->SetJson(jsonData1);
            SIMPLE_2D_LOG_INFO << "Player is not jumping anymore";
        } else if (jsonData2.contains("type") && jsonData2["type"] == "enemy") {
            auto motionComponent1 = std::static_pointer_cast<simple_2d::MotionComponent>(engine.GetComponentManager("motion")->GetComponent(entityId1));
            auto collisionBody1 = std::static_pointer_cast<simple_2d::CollisionBodyComponent>(engine.GetComponentManager("collision_body")->GetComponent(entityId1));
            switch (collisionType) {
                case simple_2d::CollisionBodyComponent::CollisionType::Cb1BottomEdgeCollidingWithCb2TopEdge:
                    motionComponent1->SetVelocityOneAxis(simple_2d::Axis::Y, -JUMP_INITIAL_SPEED_WHEN_HIT_ENEMY);
                    SIMPLE_2D_LOG_INFO << "Player is hit enemy";
                    break;
                default:
                    motionComponent1->SetVelocityOneAxis(simple_2d::Axis::Y, -JUMP_INITIAL_SPEED_WHEN_HIT_ENEMY);
                    collisionBody1->SetEnabled(false);
                    SIMPLE_2D_LOG_INFO << "Oh no. Player hit at side of enemy. Lose the game";
                    break;
            }
        }
    });
    return simple_2d::Error::OK;
}


