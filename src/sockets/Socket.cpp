#include <Socket.hpp>

MySocket::MySocket( bool initializeSocket ) {
    // Request Winsock
    WORD wVersionRequested = MAKEWORD(2,2);
    // WSAStartup returns 0 if successfull
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    // Check if dll exists
    if(wsaerr != 0){ 
        cout << "The Winsock dll not found!" << endl;
        exit(-1);
    }

    this->currentSocket = INVALID_SOCKET; // initializing as a inivalid socket
    
    if (initializeSocket) {
        // AF_INET - use IPv4 protocol
        // SOCK_STREAM - use TCP 
        this->currentSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        // Check if creating socket is successfull or not
        if(this->currentSocket == INVALID_SOCKET){
            // cout << "Error at socket():"<< WSAGetLastError() << endl;
            throw std::runtime_error("Error at socket() :" + std::to_string(WSAGetLastError()));
            // Clean and exit on error
            // WSACleanup();
        }
    }
}

MySocket::~MySocket() {
    // Close socket and clean up
    closesocket(currentSocket);
    WSACleanup();
}

// Send to address
void MySocket::sendTo( const char * buffer, const sockaddr * addr ) {
    // Send data
    int sbyteCount = sendto( this->currentSocket, buffer, BUFFER_SIZE, 0, addr, sizeof(*addr) );
    // Check if sending is successfull
    if(sbyteCount == SOCKET_ERROR){
        cout << "Send error: " << WSAGetLastError() << endl;
        throw std::runtime_error("Send error");
    }
}

// Receive from address
std::string MySocket::receiveFrom( sockaddr * addr ) {
    // buffer
    char buffer[BUFFER_SIZE];
    // address len
    int fromLen = sizeof(*addr);
    // Receive
    int rbyteCount = recvfrom( this->currentSocket, buffer, BUFFER_SIZE, 0, addr, &fromLen );

    // Check if receiving is successfull
    if(rbyteCount < 0){
        cout << "Server recv error: " << WSAGetLastError() << endl;
        throw std::runtime_error("Server recv error");
    }

    // Add null terminator to secure string
    buffer[rbyteCount] = '\0';
    // Return
    return std::string(buffer);
}

// Return socket
const SOCKET& MySocket::getSocket() { return this->currentSocket; }  

// Return sockaddr ptr
sockaddr* MySocket::getSockaddrPtr() { return (sockaddr *)&(this->service); }
const sockaddr* MySocket::getSockaddrPtrConst() { return (sockaddr *)&(this->service); }

// Receive
std::string MySocket::receive() {
    // buffer
    char buffer[BUFFER_SIZE];
    // Receive
    int rbyteCount = recv(this->currentSocket, buffer, BUFFER_SIZE, 0);

    // Check if receiving is successfull
    if(rbyteCount < 0){
        cout << "Server recv error: " << WSAGetLastError() << endl;
        throw std::runtime_error("Server recv error");
    }

    // Add null terminator to secure string
    buffer[rbyteCount] = '\0';
    // Return
    return std::string(buffer);
}

// Send
void MySocket::_send( const char * buffer ) {
    // Send data
    int sbyteCount = send(this->currentSocket, buffer, BUFFER_SIZE, 0);
    // Check if sending is successfull
    if(sbyteCount == SOCKET_ERROR){
        cout << "Server send error: " << WSAGetLastError() << endl;
        throw std::runtime_error("Server send error");
    }
}