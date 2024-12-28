#pragma once

// Include Config
#include <Config.hpp>

class MySocket {
protected:
    // For checking availability
    WSADATA wsaData;
    // For error     
    int wsaerr;
    // Socket 
    SOCKET currentSocket;
    // Service
    sockaddr_in service;

public:
    MySocket( bool initializeSocket = true );
    virtual ~MySocket();

    // Useable in Server
    virtual void startListening() {}
    virtual void startRespondingForBroadcast() {}
    virtual void stopRespondingForBroadcast() {}
    virtual void shutdownServer() {}

    // Useable in Connected Socket
    virtual bool connectToServer( const SOCKET & serverSocket ) { return false; }
    virtual void disconnectFromServer() {}

    // Return socket
    const SOCKET & getSocket(); 
    // Return sockaddr ptr
    const sockaddr * getSockaddrPtrConst();
    sockaddr * getSockaddrPtr();

    void sendTo( const char * buffer, const sockaddr * addr );
    std::string receiveFrom( sockaddr * addr );

    void _send( const char * buffer );
    std::string receive();
};