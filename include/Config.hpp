#pragma once

#define RAYMATH_IMPLEMENTATION
#include <raylib.h>

// Include all needed windows libraries
// Fix redefinitions
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define CloseWindow WindowsCloseWindow
#define ShowCursor WindowsShowCursor
#define Rectangle WindowsRectangle
#include <winsock2.h>
#include <ws2tcpip.h>
#undef CloseWindow
#undef ShowCursor
#undef Rectangle

#include <iostream>
#include <tchar.h>
#include <sstream>
#include <conio.h>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <stack>
#include <functional>

// Include all needed namespaces
using namespace std;

// Define all needed constants
#define PORT 48051
#define TIMEOUT_MS 2000
#define BUFFER_SIZE 256
#define DISCOVER "DISCOVER"

// Define all needed macros
#define RANDOM(FROM, TO) rand() % (TO - FROM + 1) + FROM
#define TEXT_SPACING 2.f

// Define colors
#define MY_LIGHTGRAY Color{217, 217, 217, 255}
#define MY_DARKGRAY Color{166, 166, 166, 255}
#define MY_YELLOW Color{255, 222, 89, 255}
#define MY_ORANGE Color{255, 189, 89, 255}

class ScreenSide {
public:
    enum Value {
        LEFT = 0,
        RIGHT = 1,
        TOP = 2,
        BOTTOM = 3
    };
};