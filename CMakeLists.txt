cmake_minimum_required(VERSION 3.5)
project(flappy-bird-2d)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(simple-2d EXCLUDE_FROM_ALL)

add_executable(platform_2d
    src/main.cpp
    src/enemy.cpp
    src/ground.cpp
    src/player.cpp
)


target_link_libraries(platform_2d PRIVATE simple-2d)

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

install(FILES assets/ground_tile.png
     DESTINATION assets
)

install(FILES assets/sine.wav
     DESTINATION assets
)
