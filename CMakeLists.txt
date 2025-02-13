cmake_minimum_required(VERSION 3.29)
project(RockPaperScisors VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)

# Dependencies
add_subdirectory(include/external/raylib)

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
    src/sprites/RotatingSprite.cpp
    src/sprites/TextButton.cpp
    src/Background.cpp
    src/sprites/Button.cpp
    src/InputField.cpp
    src/scenes/HostLobbyScene.cpp
    src/scenes/ServerListScene.cpp
    src/Text.cpp
    src/scenes/GameScene.cpp
    src/scenes/GameLoadingScene.cpp
    main.cpp
)

include_directories(
    ${RAYLIB_INCLUDE_DIRS} 
    include 
    include/sockets 
)


target_compile_features(RockPaperScisors PUBLIC cxx_std_17)
target_link_libraries(RockPaperScisors PUBLIC raylib wsock32 ws2_32 -static)


