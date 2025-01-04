#pragma once

#include <Socket.hpp>
#include <Config.hpp>

// Client 
class Client : public MySocket {
public:
    // Constructor address of server to connect to 
    // Connects to server and set socket for communication
    Client(const char * addr);
    void disconnectFromServer();
    // const SOCKET & getSocket() { return this->currentSocket; } <- from MySocket
};