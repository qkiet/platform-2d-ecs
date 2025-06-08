#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <chrono>
#include <thread>
#include <vector>
#include <simple-2d/core.h>
#include <memory>
#include <simple-2d/entity.h>
#include <simple-2d/scene.h>
#include <simple-2d/component.h>
#include <simple-2d/components/static_sprite.h>
#include <simple-2d/components/motion.h>
#include <simple-2d/components/downward_gravity.h>
#include <simple-2d/components/animated_sprite.h>
#include <SDL3/SDL_events.h>
#include "player.h"
#include "ground.h"
#include "enemy.h"

#define TICK_PER_SEC 60
#define TICK_INTERVAL_MSEC 1000 / TICK_PER_SEC


static uint32_t get_remaining_ticks(const std::chrono::system_clock::time_point &last_tick_ts ) {
    const auto current_ts = std::chrono::high_resolution_clock::now();
    const auto elapsed_msec = std::chrono::duration_cast<std::chrono::milliseconds>(current_ts - last_tick_ts);
    return elapsed_msec / std::chrono::milliseconds(TICK_INTERVAL_MSEC);
}


int main(int argc, char *argv[]) {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::debug);

    auto &engine = simple_2d::Engine::GetInstance();
    engine.Init("Flappy Bird", 800, 600, simple_2d::Color{255, 255, 255, 255});
    auto lastTickTimestamp = std::chrono::high_resolution_clock::now();
    auto scene = std::make_shared<simple_2d::Scene>(simple_2d::RectangularDimensions<int>{800, 600});
    engine.SetCurrentScene(scene);
    Player player;
    player.Init();
    Ground ground;
    ground.Init();
    Enemy enemy1;
    enemy1.Init();
    engine.GetCamera().SetPosition(simple_2d::XYCoordinate<float>(100, 100));
    while (true) {

        auto remainTicks = get_remaining_ticks(lastTickTimestamp);
        if (!remainTicks) {
            continue;
        }
        auto processTicks = remainTicks;
        auto isQuit = false;
        while (processTicks--) {
            auto error = engine.Step();
            if (simple_2d::Error::QUIT == error) {
                isQuit = true;
                break;
            }
        }
        if (isQuit) {
            break;
        }
        lastTickTimestamp = std::chrono::high_resolution_clock::now();
    }
    engine.Deinit();
    return 0;
}
