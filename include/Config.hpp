#pragma once

// Include all needed libraries
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

// Include all needed namespaces
using namespace std;

// Define all needed constants
#define PORT 48051
#define TIMEOUT_MS 2000
#define BUFFER_SIZE 256
#define DISCOVER "DISCOVER"