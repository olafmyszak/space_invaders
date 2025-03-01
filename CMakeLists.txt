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
        src/Spaceship.h
        src/BulletManager.h
        src/Alien.h
        src/GameManager.h
        src/AlienManager.h
)

# Define common compile options
set(COMMON_COMPILE_OPTIONS "-Wall")

# Define GCC-specific compile options
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(GCC_COMPILE_OPTIONS "-Wextra" "-Wfloat-equal" "-Wpointer-arith" "-Wunreachable-code" "-Winit-self" "-Wuninitialized")
    set(GCC_COMPILE_DEBUG_OPTIONS ${GCC_COMPILE_OPTIONS} "-g" "-Og")
    set(GCC_COMPILE_RELEASE_OPTIONS ${GCC_COMPILE_OPTIONS} "-O3" "-flto")

    target_compile_options(space_invaders PRIVATE
            ${COMMON_COMPILE_OPTIONS}
            "$<$<CONFIG:Debug>:${GCC_COMPILE_DEBUG_OPTIONS}>"
            "$<$<CONFIG:Release>:${GCC_COMPILE_RELEASE_OPTIONS}>"
    )
endif ()

# Define MSVC-specific compile options
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(MSVC_COMPILE_OPTIONS "/W3" "/permissive-")
    set(MSVC_COMPILE_DEBUG_OPTIONS ${MSVC_COMPILE_OPTIONS} "/Zi" "/Od")
    set(MSVC_COMPILE_RELEASE_OPTIONS ${MSVC_COMPILE_OPTIONS} "/O2" "/GL")

    target_compile_options(space_invaders PRIVATE
            ${COMMON_COMPILE_OPTIONS}
            "$<$<CONFIG:Debug>:${MSVC_COMPILE_DEBUG_OPTIONS}>"
            "$<$<CONFIG:Release>:${MSVC_COMPILE_RELEASE_OPTIONS}>"
    )
endif ()


target_compile_features(space_invaders PRIVATE cxx_std_17)
target_link_libraries(space_invaders PRIVATE SFML::Graphics)

