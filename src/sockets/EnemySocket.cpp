#include <EnemySocket.hpp>

EnemySocket::EnemySocket( const SOCKET & serverSocket ) 
    : MySocket(false) /* Doesn't need to be initialized so it's false */{
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
        exit(-1);
    }

    cout << "Connected with player!" << endl;
}