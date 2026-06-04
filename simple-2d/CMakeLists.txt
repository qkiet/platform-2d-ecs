cmake_minimum_required(VERSION 3.5)
project(simple-2d)

set (CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(SDL_SHARED OFF)
set(SDL_STATIC ON)
set(BUILD_SHARED_LIBS OFF)
set(vendored_default ON)

add_subdirectory(${PROJECT_SOURCE_DIR}/3rd-party/SDL)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rd-party/SDL_image)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rd-party/SDL_mixer)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rd-party/json)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rd-party/plog)

set(SIMPLE_2D_SRCS
    src/core.cpp
    src/graphics.cpp
    src/audio.cpp
    src/internal_utils.cpp
    src/component.cpp
    src/entity.cpp
    src/camera.cpp
    src/scene.cpp
    src/geometry.cpp
    src/components/static_sprite.cpp
    src/components/motion.cpp
    src/components/downward_gravity.cpp
    src/components/animated_sprite.cpp
    src/components/json.cpp
    src/components/behavior_script.cpp
    src/components/static_repetitive_sprite.cpp
    src/components/collision_body.cpp
)

add_library(simple-2d
    ${SIMPLE_2D_SRCS}
)

target_include_directories(simple-2d PUBLIC include)



# Export these library in order for imported CMake projects to use these included directories as well
target_link_libraries(simple-2d PUBLIC SDL3::SDL3 SDL3_image::SDL3_image SDL3_mixer::SDL3_mixer nlohmann_json::nlohmann_json plog::plog)

