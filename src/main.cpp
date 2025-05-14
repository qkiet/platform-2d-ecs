#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <chrono>
#include <thread>
#include <vector>
#include <simple-2d/core.h>
#include <memory>

#define TICK_PER_SEC 60
#define TICK_INTERVAL_MSEC 1000 / TICK_PER_SEC


static uint32_t get_remaining_ticks(const std::chrono::system_clock::time_point &last_tick_ts ) {
    const auto current_ts = std::chrono::high_resolution_clock::now();
    const auto elapsed_msec = std::chrono::duration_cast<std::chrono::milliseconds>(current_ts - last_tick_ts);
    return elapsed_msec / std::chrono::milliseconds(TICK_INTERVAL_MSEC);
}


int main(int argc, char *argv[]) {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::debug);

    std::unique_ptr<simple_2d::Engine> engine = std::make_unique<simple_2d::Engine>();
    engine->Init("Flappy Bird", 800, 600, simple_2d::Color{255, 255, 255, 255});
    auto last_tick_ts = std::chrono::high_resolution_clock::now();
    while (true) {

        auto remain_ticks = get_remaining_ticks(last_tick_ts);
        if (!remain_ticks) {
            continue;
        }
        auto processing_ticks = remain_ticks;
        std::vector<SDL_Event> events_to_handle;
        SDL_Event event;
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            BOOST_LOG_TRIVIAL(info) << "Receive quit";
            break;
        }
        if (event.type != SDL_EVENT_POLL_SENTINEL) {
            events_to_handle.push_back(event);
        }
        while (processing_ticks--) {
            engine->Step();
        }
        events_to_handle.clear();
        last_tick_ts = std::chrono::high_resolution_clock::now();
    }
    return 0;
}
