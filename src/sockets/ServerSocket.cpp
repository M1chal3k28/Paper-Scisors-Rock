#include <ServerSocket.hpp>

// Constructor name of the server which displays on the screen when searching by other players
PlayerServer::PlayerServer(const std::string & name) : MySocket(), serverName(name) {
    this->service = sockaddr_in(); 
    this->service.sin_family = AF_INET; // ipv4
    this->service.sin_addr.s_addr = INADDR_ANY; // Listen on all addresses
    this->service.sin_port = htons(PORT); // PORT specified in config
    // Using the bind function
    if( bind( this->currentSocket, (SOCKADDR*)&( this->service ), sizeof( this->service ) ) == SOCKET_ERROR){
        cout << "bind() failed: " << WSAGetLastError() << endl;
        // Close socket and clean up on error
        closesocket(currentSocket);
        // WSACleanup();
        throw std::runtime_error("Failed to bind socket");
    }

    // Create socket for broadcast
    this->broadcastSocket = socket( AF_INET, SOCK_DGRAM, 0 ); // UDP socket
    if ( this->broadcastSocket == INVALID_SOCKET ) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << endl;
        closesocket(currentSocket);
        // WSACleanup();
        throw std::runtime_error("Socket creation failed");
    }

    // Set timeout for data receiving (recvfrom)
    int timeout = TIMEOUT_MS;
    setsockopt( this->broadcastSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout) );

    // bind to broadcast socket
    if ( bind( this->broadcastSocket, (const sockaddr*)&(this->service), sizeof(this->service) ) == SOCKET_ERROR ) {
        cout << "bind() for broadcast failed: " << WSAGetLastError() << endl;
        // Close socket and clean up on error
        closesocket(currentSocket);
        // WSACleanup();
        throw std::runtime_error("Failed to bind broadcast socket");
    }
}

// Destructor Clears the socket and thread
PlayerServer::~PlayerServer() {
    this->stopRespondingForBroadcast();
    
    // Close sockets
    closesocket(this->broadcastSocket);
    closesocket(this->currentSocket);
}

// Function that runs in a separate thread for responding to broadcast
void PlayerServer::responseForBroadcast() {
    // Receive data
    char buffer[BUFFER_SIZE] = {0};
    // Receive data from broadcast
    sockaddr_in clientAddr {};
    // Size of client address
    int clientAddrLen = sizeof(sockaddr_in);

    // Receive data while running
    while ( this->responding ) {
        // Receive data and who sent
        int n = recvfrom(this->broadcastSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&clientAddr, &clientAddrLen);
        
        // If data received
        if (n > 0) {
            // Secure data
            buffer[n] = '\0';

            // Respond if message is DISCOVER
            if (std::string(buffer) == DISCOVER) {
                std::string response = this->serverName + "'s server";
                sendto(this->broadcastSocket, response.c_str(), response.size(), 0, (sockaddr*)&clientAddr, clientAddrLen);
            }
        }
    }
}

// Listen for connections
void PlayerServer::startListening() {
    // Using the listen function
    if( listen( this->currentSocket, 1 ) == SOCKET_ERROR ){
        cout << "listen(): Error listening on socket: " << WSAGetLastError() << endl;
        // clean up and exit on error
        closesocket(this->currentSocket);
        // WSACleanup();
        throw runtime_error("listen() failed");
    } 
        
    cout << "Waiting for second player to connect..." << endl;
}

void PlayerServer::shutdownServer() {
    this->stopRespondingForBroadcast();

    // Close sockets
    if (this->currentSocket != INVALID_SOCKET)
        closesocket(this->currentSocket);
    if (this->broadcastSocket != INVALID_SOCKET)
        closesocket(this->broadcastSocket);
    // WSACleanup();
}

// Start responding to broadcasts
void PlayerServer::startRespondingForBroadcast() {
    this->responding = true;
    this->responseFuture = std::async(std::launch::async, &PlayerServer::responseForBroadcast, this);
}

// Stop responding to broadcasts
void PlayerServer::stopRespondingForBroadcast() {
    this->responding = false;
    if (this->responseFuture.valid()) {
        // Wait for thread to finish
        this->responseFuture.get();
    }
}

