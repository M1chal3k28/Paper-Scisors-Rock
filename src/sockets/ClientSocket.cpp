#include <ClientSocket.hpp>

// Constructor address of server to connect to 
// Connects to server and set socket for communications
Client::Client(const char * addr) 
    : MySocket() {
    // Set service address and port
    this->service = sockaddr_in(); 
    this->service.sin_family = AF_INET;
    this->service.sin_addr.s_addr = inet_addr(addr); // Address of the server
    this->service.sin_port = htons(PORT); // PORT set in config
    // Using the bind function
    if( connect( this->currentSocket, (SOCKADDR*)&( this->service ), sizeof( this->service ) ) == SOCKET_ERROR){
        cout << "Connection with server failed: " << WSAGetLastError() << endl;
        // Close socket and clean up on error
        closesocket(currentSocket);
        // WSACleanup();
        throw std::runtime_error("Connection with server failed");
    }

    cout << "Connected to The server !" << endl;
}

void Client::disconnectFromServer() {
    if(this->currentSocket != INVALID_SOCKET) {
        closesocket(this->currentSocket);
        std::cout << "Disconnected from server " << std::endl;
    }
}
