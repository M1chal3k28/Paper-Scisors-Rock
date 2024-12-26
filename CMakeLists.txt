cmake_minimum_required(VERSION 3.29)
project(RockPaperScisors VERSION 0.1.0 LANGUAGES C CXX)

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_MakeAvailable(raylib)
endif()

add_executable(RockPaperScisors 
    src/sockets/BroadcastSocket.cpp
    src/sockets/ClientSocket.cpp
    src/sockets/Socket.cpp
    src/sockets/ServerSocket.cpp
    src/sockets/EnemySocket.cpp
    src/Player.cpp
    src/Game.cpp
    src/ResourceManager.cpp
    src/scenes/IntroScene.cpp
    src/scenes/SceneManager.cpp
    src/scenes/SetupScene.cpp
    src/sprites/Sprite.cpp
    src/sprites/MovingSprite.cpp
    src/Background.cpp
    main.cpp
)

include_directories(
    ${RAYLIB_INCLUDE_DIRS} 
    include 
    include/sockets 
)
target_compile_features(RockPaperScisors PUBLIC cxx_std_17)
target_link_libraries(RockPaperScisors PRIVATE raylib wsock32 ws2_32)
