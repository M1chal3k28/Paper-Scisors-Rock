#pragma once

// Include all needed libraries
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
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