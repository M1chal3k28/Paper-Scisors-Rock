#pragma once

#include <Config.hpp>
#include <Socket.hpp>

// Accepted enemy
class EnemySocket : public MySocket {
    // address of accepted connection
    sockaddr addressOfConnection;
public:
    // Accept connection and set socket for communication with player
    EnemySocket();
    ~EnemySocket();

    bool connectToServer( const SOCKET & serverSocket );
    void disconnectFromServer();
    // const SOCKET & getSocket() { return this->currSocket; } <- from MySocket
};