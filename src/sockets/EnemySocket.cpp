#include <EnemySocket.hpp>

EnemySocket::EnemySocket( ) 
    : MySocket(false) /* Doesn't need to be initialized so it's false */{
}

bool EnemySocket::connectToServer( const SOCKET & serverSocket ) {
    // Accept connection
    // len of address
    socklen_t addrLen = sizeof(this->addressOfConnection);
    // Enemy socket will have the address of the client
    this->currentSocket = accept(serverSocket, (struct sockaddr*)&(this->addressOfConnection), &addrLen);

    // Check if accept is successfull
    if(this->currentSocket == INVALID_SOCKET){
        cout << "Accept failed: " << WSAGetLastError() << endl;
        // On error clean up and exit
        WSACleanup();
        throw std::runtime_error("Accept failed");
        return false; // <- this has to be 
    }

    cout << "Connected with player!" << endl;
    return true;
}
