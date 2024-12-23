cmake_minimum_required(VERSION 3.5.0)
project(RockPaperScisors VERSION 0.1.0 LANGUAGES C CXX)

add_executable(RockPaperScisors 
    src/sockets/BroadcastSocket.cpp
    src/sockets/ClientSocket.cpp
    src/sockets/Socket.cpp
    src/sockets/ServerSocket.cpp
    src/sockets/EnemySocket.cpp
    src/Player.cpp
    src/Game.cpp
    main.cpp
)

# Add raylib
include_directories(include include/sockets)
add_subdirectory(include/external/raylib)
add_subdirectory(include)

target_compile_features(RockPaperScisors PUBLIC cxx_std_17)

target_link_libraries(RockPaperScisors wsock32 ws2_32)


