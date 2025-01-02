#pragma once

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
#undef DrawText
#undef ShowCursor
#undef Rectangle
#undef DrawTextEx

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
#include <future>

// Include all needed namespaces
using namespace std;

// Define all needed constants
#define PORT 48051
#define TIMEOUT_MS 1000
#define BUFFER_SIZE 256
#define DISCOVER "DISCOVER"

// Define all needed macros
#define RANDOM(FROM, TO) rand() % (TO - FROM + 1) + FROM
#define TEXT_SPACING 2.f
#define TEXT_SIZE 40.f
#define TITLE_SIZE 150.f

// Define colors
#define MY_LIGHTGRAY Color{217, 217, 217, 255}
#define MY_DARKGRAY Color{166, 166, 166, 255}
#define MY_YELLOW Color{255, 222, 89, 255}
#define MY_ORANGE Color{255, 189, 89, 255}

// Resources
#define DEFAULT_FONT_KEY "default"
#define DEFAULT_TEXTURE_KEY "default"
#define DEFAULT_FONT *RESOURCE_MANAGER.getFont(DEFAULT_FONT_KEY);
#define DEFAULT_TEXTURE *RESOURCE_MANAGER.getTexture(DEFAULT_TEXTURE_KEY);

// Minecraft font
#define MINECRAFT_FONT_KEY "minecraft-font"

// Buttons
#define BUTTON_MAX_STATES 2 // 0 is not hovered, 1 is hovered 2 is pressed

// Button txt name
#define BUTTON_TXT_KEY "button"

// Define button sizes and offsets
#define MENU_BUTTON_SIZE {600.f, 100.f}
// Define all offsets for this type
#define MENU_BUTTON_SHEET_OFFSET {0.f, 0.f}

#define SMALL_BUTTON_SIZE {100.f, 100.f}
// Define all offsets for this type
#define SMALL_BUTTON_EXIT_OFFSET {0.f, 100.f}
#define SMALL_BUTTON_RELOAD_OFFSET {0.f, 200.f}
#define SMALL_BUTTON_PAPER_OFFSET {300.f, 100.f}
#define SMALL_BUTTON_ROCK_OFFSET {600.f, 100.f}
#define SMALL_BUTTON_SCISSORS_OFFSET {900.f, 100.f}
#define SMALL_BUTTON_HOME_OFFSET {300.f, 200.f}

#define TINY_BUTTON_SIZE {50.f, 50.f}
// Define all offsets for this type
#define TINY_BUTTON_LEFTARROW_OFFSET {0.f, 300.f}
#define TINY_BUTTON_RIGHTARROW_OFFSET {150.f, 300.f}

// Screen side enum class
class ScreenSide {
public:
    enum Value {
        LEFT = 0,
        RIGHT = 1,
        TOP = 2,
        BOTTOM = 3
    };
};

// Player type enum class
class PlayerType {
public:
    enum Value {
        Host = 0,
        Client = 1,
        Offline = 2,
        Enemy = 3
    };
};

// Type of the scene
class SceneType {
public:
    enum Value {
        INTRO = 0,
        SETUP,
        HOST_LOBBY,
        SERVER_SELECTION,
        GAME_SCENE
    };    
};