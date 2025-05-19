#include "player.h"
#include <simple-2d/core.h>
#include <boost/log/trivial.hpp>
#include <simple-2d/components/animated_sprite.h>
#include <simple-2d/components/motion.h>
#include <simple-2d/components/behavior_script.h>
#include <simple-2d/components/json.h>

#define MOVE_SPEED_PER_TICKS 3
#define JUMP_INITIAL_SPEED 15

static void onKeyPressedEvent(simple_2d::EntityId entityId, const SDL_Event& event) {
    auto &engine = simple_2d::Engine::GetInstance();
    auto json = std::static_pointer_cast<simple_2d::JsonComponent>(engine.GetComponentManager("json")->GetComponent(entityId));
    auto jsonData = json->GetJson();
    BOOST_LOG_TRIVIAL(debug) << "Key pressed event";
    if (event.key.scancode == SDL_SCANCODE_LEFT) {
        jsonData["isMovingLeft"] = true;
        BOOST_LOG_TRIVIAL(debug) << "Left key pressed";
    } else if (event.key.scancode == SDL_SCANCODE_RIGHT) {
        jsonData["isMovingRight"] = true;
        BOOST_LOG_TRIVIAL(debug) << "Right key pressed";
    }
    json->SetJson(jsonData);
}

static void onKeyReleasedEvent(simple_2d::EntityId entityId, const SDL_Event& event) {
    auto &engine = simple_2d::Engine::GetInstance();
    BOOST_LOG_TRIVIAL(debug) << "Key released event";
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
    BOOST_LOG_TRIVIAL(debug) << "Tick event";
    auto motionComponentManager = simple_2d::Engine::GetInstance().GetComponentManager("motion");
    if (motionComponentManager == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "Failed to get motion component manager";
        return;
    }
    auto component = motionComponentManager->GetComponent(entityId);
    if (component == nullptr) {
        BOOST_LOG_TRIVIAL(error) << "Failed to get motion component for entity " << entityId;
        return;
    }
    auto motionComponent = std::static_pointer_cast<simple_2d::MotionComponent>(component);
    auto isMovingRight = jsonData["isMovingRight"];
    auto isMovingLeft = jsonData["isMovingLeft"];
    auto wantToJump = jsonData["wantToJump"];
    auto isJumping = jsonData["isJumping"];
    int direction = (int)isMovingRight - (int)isMovingLeft;
    BOOST_LOG_TRIVIAL(debug) << "Direction: " << direction;
    int aboutToJump = wantToJump && !isJumping;
    BOOST_LOG_TRIVIAL(debug) << "IsJumping: " << isJumping;
    motionComponent->SetVelocityOneAxis(simple_2d::Axis::X, direction * MOVE_SPEED_PER_TICKS);
    if (aboutToJump) {
        BOOST_LOG_TRIVIAL(debug) << "Jump";
        motionComponent->SetVelocityOneAxis(simple_2d::Axis::Y, -JUMP_INITIAL_SPEED);
        jsonData["isJumping"] = true;
    }
    json->SetJson(jsonData);
}

simple_2d::Error Player::Init() {
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
    error = AddComponent("behavior_script");
    if (error != simple_2d::Error::OK) {
        BOOST_LOG_TRIVIAL(error) << "Failed to add behavior_script component";
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
    auto animatedSprite = std::static_pointer_cast<simple_2d::AnimatedSprite>(GetComponent("animated_sprite"));
    auto bitmap = engine.GetGraphics().LoadImageFromFile("assets/player_idle_1.png");
    animatedSprite->AddAnimation(0, bitmap.texture, 5);
    bitmap = engine.GetGraphics().LoadImageFromFile("assets/player_idle_2.png");
    animatedSprite->AddAnimation(0, bitmap.texture, 5);
    bitmap = engine.GetGraphics().LoadImageFromFile("assets/player_idle_3.png");
    animatedSprite->AddAnimation(0, bitmap.texture, 5);
    animatedSprite->PlayAnimation(0);
    auto motion = std::static_pointer_cast<simple_2d::MotionComponent>(GetComponent("motion"));
    motion->SetPosition(simple_2d::XYCoordinate<float>(100, 100));
    auto json = std::static_pointer_cast<simple_2d::JsonComponent>(GetComponent("json"));
    json->SetJson(nlohmann::json::parse(R"(
        {
            "isMovingLeft": false,
            "isMovingRight": false,
            "wantToJump": false,
            "isJumping": false
        }
    )"));
    auto behaviorScript = std::static_pointer_cast<simple_2d::BehaviorScript>(GetComponent("behavior_script"));
    behaviorScript->SetOnKeyPressedEventCallback(onKeyPressedEvent);
    behaviorScript->SetOnKeyReleasedEventCallback(onKeyReleasedEvent);
    behaviorScript->SetOnTickEventCallback(onTickEvent);
    return simple_2d::Error::OK;
}


