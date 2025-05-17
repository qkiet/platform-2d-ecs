cmake_minimum_required(VERSION 3.5)
project(flappy-bird-2d)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(3rd-party/SDL EXCLUDE_FROM_ALL)
add_subdirectory(3rd-party/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(3rd-party/SDL_mixer EXCLUDE_FROM_ALL)

set(SIMPLE_2D_SRCS
        simple-2d/src/core.cpp
        simple-2d/src/graphics.cpp
        simple-2d/src/audio.cpp
        simple-2d/src/internal_utils.cpp
        simple-2d/src/component.cpp
        simple-2d/src/entity.cpp
        simple-2d/src/components/static_sprite.cpp
        simple-2d/src/components/motion.cpp
        simple-2d/src/components/platform_player.cpp
        simple-2d/src/components/downward_gravity.cpp
        simple-2d/src/components/animated_sprite.cpp
)

add_executable(platform_2d
        src/main.cpp
        ${SIMPLE_2D_SRCS}
        # src/logic.cpp
)

target_include_directories(platform_2d PRIVATE simple-2d/include)

target_link_libraries(platform_2d PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_mixer::SDL3_mixer boost_system boost_log)

target_compile_options(platform_2d PRIVATE -DBOOST_LOG_DYN_LINK)

install(TARGETS platform_2d
)
install(FILES assets/player_idle_1.png
        DESTINATION assets
)
install(FILES assets/player_idle_2.png
        DESTINATION assets
)
install(FILES assets/player_idle_3.png
        DESTINATION assets
)

install(FILES assets/tile.png
        DESTINATION assets
)

install(FILES assets/sine.wav
        DESTINATION assets
)
