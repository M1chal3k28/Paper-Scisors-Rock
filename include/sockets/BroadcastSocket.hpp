#pragma once

#include <Config.hpp>

class BroadcastSocket {
    // Socket of broadcast
    SOCKET broadcastSocket;
    // Adress of broadcast
    sockaddr_in broadcastAddr;
    // Special message which will be sent to all devices with port PORT
    const char * message = DISCOVER;
    // For error during creating socket
    WSADATA wsaData;

    // List of servers with address of host
    std::vector<std::pair<std::string, sockaddr_in>> discoveredServers;
    // Is searching for servers
    bool running;
    // Thread for searching
    std::thread discoverThread;

    // Function for searching for servers
    void searchForServers();
public:
    BroadcastSocket();
    ~BroadcastSocket();

    // Starts the broadcast in a separate thread
    void startBroadcast();

    // Stops the broadcast and the thread
    std::vector<std::pair<std::string, sockaddr_in>> getResults();
};