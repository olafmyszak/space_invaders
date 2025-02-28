cmake_minimum_required(VERSION 3.28)
project(space_invaders LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(space_invaders src/main.cpp
        src/Bullet.h
        src/BulletType.h
        src/BulletFactory.h
        src/Spaceship.h)
target_compile_features(space_invaders PRIVATE cxx_std_17)
target_link_libraries(space_invaders PRIVATE SFML::Graphics)

