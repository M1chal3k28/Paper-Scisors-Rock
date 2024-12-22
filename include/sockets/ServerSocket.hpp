#pragma once

#include <Socket.hpp>
#include <Config.hpp>

// Server when creating game
class PlayerServer : public MySocket {
    // server name
    std::string serverName;

    // Socket for responding to broadcast
    SOCKET broadcastSocket;
    // Thread for responding to broadcast
    std::thread responseThread;
    // Is responding for broadcast. Thread running?
    bool responding = false;

    // Function that runs in a separate thread for responding to broadcast
    void responseForBroadcast();
public:
    // Constructor name of the server which displays on the screen when searching by other players
    PlayerServer(const std::string & name);
    ~PlayerServer();

    // Listen for connections
    virtual void startListening();

    // Start responding to broadcasts
    virtual void startRespondingForBroadcast();

    // Stop responding to broadcasts
    virtual void stopRespondingForBroadcast();
};