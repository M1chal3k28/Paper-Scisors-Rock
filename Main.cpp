#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <sstream>
#include <conio.h>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
using namespace std;

#define PORT 55555
#define TIMEOUT_MS 2000
#define BUFFER_SIZE 256
#define DISCOVER "DISCOVER"

template<typename T>
T getValue( const char * message = "" ) {
    string _buff;
    T _return;
    
    stringstream _input;
    _input.clear( );
    _input.str( "" );
    
    while( 1 ) {
        cout << message << " _>";
        if( !getline( cin, _buff ) ) {
            cerr << "Error While Getting Input !";
            exit(-1);
        }
        
        _input.str( _buff );
        
        if ( _input >> _return )
            break;
            
        cerr << "\033[31m" << "Wrong input" << "\033[0m\n";
        
        _input.clear();
        _input.str( "" );
    }
    
    return _return;
}

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
    MySocket() {
        // Check if dll exists
        WORD wVersionRequested = MAKEWORD(2,2);
        wsaerr = WSAStartup(wVersionRequested, &wsaData);

        if(wsaerr != 0){ 
            cout << "The Winsock dll not found!" << endl;
            exit(-1);
        }

        this->currentSocket = INVALID_SOCKET; // initializing as a inivalid socket
        // AF_INET - use IPv4 protocol
        // SOCK_STREAM - use TCP 
        this->currentSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        // Check if creating socket is successfull or not
        if(this->currentSocket == INVALID_SOCKET){
            cout << "Error at socket():"<< WSAGetLastError() << endl;
            // Clean and exit on error
            WSACleanup();
            exit(-1);
        }
    }

    ~MySocket() {
        // Close socket and clean up
        closesocket(currentSocket);
        WSACleanup();
    }

    // Useable in Server
    virtual void startListening() {}
    virtual void startRespondingForBroadcast() {}
    virtual void stopRespondingForBroadcast() {}
    
    // Return socket
    const SOCKET & getSocket() {
        return this->currentSocket;
    }  
};

class BroadcastSocket {
    // Socket of broadcast
    SOCKET broadcastSocket;
    // Adress of broadcast
    sockaddr_in broadcastAddr;
    // Special message which will be sent to all devices with port 55555
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
    void searchForServers() {
        // Buffer for receiving data
        char buffer[BUFFER_SIZE];
        // Address of sender (server)
        sockaddr_in serverAddr {};
        // Length of address
        int serverAddrLen = sizeof(serverAddr);

        // Search for servers while running
        do {
            discoveredServers.clear(); // Clear discovered servers

            // Send broadcast message
            sendto(this->broadcastSocket, message, strlen(message), 0, (sockaddr *)&(this->broadcastAddr), sizeof(this->broadcastAddr));

            while (true) {
                // Receive information
                int n = recvfrom(this->broadcastSocket, buffer, BUFFER_SIZE, 0, (sockaddr *)&serverAddr, &serverAddrLen);

                if (n > 0) {
                    // Add null terminator to secure string
                    buffer[n] = '\0';
                    // Convert IP to string
                    std::string serverIP = inet_ntoa(serverAddr.sin_addr);

                    // Add server to list
                    // String to server address
                    discoveredServers.emplace_back(std::make_pair(std::string(buffer), serverAddr));
                } else {
                    // No more data
                    break;
                }
            }

            // Wait for 2 seconds before next search
            if ( running )
                std::this_thread::sleep_for(std::chrono::seconds(2));
        } while ( running );
    }
public:
    BroadcastSocket() : running(false), discoveredServers() {
        // Check if dll exists / start dll
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed\n";
            return;
        }

        // Create socket for broadcast
        // ipv4, UDP
        this->broadcastSocket = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
        if (this->broadcastSocket == INVALID_SOCKET) {
            std::cerr << "Socket creation failed\n";
            WSACleanup();
            return;
        }

        // Set timeout for data receiving (recvfrom)
        int timeout = TIMEOUT_MS;
        setsockopt(this->broadcastSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

        this->broadcastAddr = sockaddr_in();
        broadcastAddr.sin_family = AF_INET;
        broadcastAddr.sin_addr.s_addr = INADDR_BROADCAST; // Broadcast to all addresses on the network on port 55555
        broadcastAddr.sin_port = htons(PORT); // Set port 55555

        // Set socket to broadcast
        BOOL broadcastEnable = TRUE;
        setsockopt(this->broadcastSocket, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastEnable, sizeof(broadcastEnable));
    }

    // Starts the broadcast in a separate thread
    void startBroadcast() {
        this->running = true;
        this->discoverThread = std::thread(&BroadcastSocket::searchForServers, this);
    }

    ~BroadcastSocket() {
        this->running = false;
        if (this->discoverThread.joinable())
            this->discoverThread.join();
        
        closesocket(this->broadcastSocket);
        WSACleanup();
    }

    // Stops the broadcast and the thread
    std::vector<std::pair<std::string, sockaddr_in>> getResults() {
        // Check if thread is running
        if ( !this->discoverThread.joinable() ) {
            throw std::runtime_error("Thread not started, could not get results from BroadcastSocket");
            return {};
        }

        // Stop searching
        this->running = false;
        // Join thread
        this->discoverThread.join();
        return this->discoveredServers;
    }
};

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
    void responseForBroadcast() {
        // Receive data
        char buffer[BUFFER_SIZE];
        // Receive data from broadcast
        sockaddr_in clientAddr {};
        // Size of client address
        int clientAddrLen = sizeof(clientAddr);

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
public:
    // Constructor name of the server which displays on the screen when searching by other players
    PlayerServer(const std::string & name) : MySocket(), serverName(name) {
        this->service = sockaddr_in(); 
        this->service.sin_family = AF_INET; // ipv4
        this->service.sin_addr.s_addr = INADDR_ANY; // Listen on all addresses
        this->service.sin_port = htons(PORT); // 55555
        // Using the bind function
        if( bind(this->currentSocket, (SOCKADDR*)&(this->service), sizeof( this->service ) ) == SOCKET_ERROR){
            cout << "bind() failed: " << WSAGetLastError() << endl;
            // Close socket and clean up on error
            closesocket(currentSocket);
            WSACleanup();
            exit(-1);
        }

        // Create socket for broadcast
        this->broadcastSocket = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
        if (this->broadcastSocket == INVALID_SOCKET) {
            std::cerr << "Socket creation failed: " << WSAGetLastError() << endl;
            closesocket(currentSocket);
            WSACleanup();
            exit(-1);
        }

        // Set timeout for data receiving (recvfrom)
        int timeout = TIMEOUT_MS;
        setsockopt(this->broadcastSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

        // bind to broadcast socket
        if (bind(this->broadcastSocket, (const sockaddr*)&(this->service), sizeof(this->service)) == SOCKET_ERROR) {
            cout << "bind() for broadcast failed: " << WSAGetLastError() << endl;
            // Close socket and clean up on error
            closesocket(currentSocket);
            WSACleanup();
            exit(-1);
        }
    }

    ~PlayerServer() {
        // Stop responding
        this->responding = false;
        if ( this->responseThread.joinable() ) 
            this->responseThread.join();
        
        // Close sockets
        closesocket(this->broadcastSocket);
        closesocket(this->currentSocket);
    }

    // Listen for connections
    virtual void startListening() {
        // Using the listen function
        if( listen( this->currentSocket, 1 ) == SOCKET_ERROR ){
            cout << "listen(): Error listening on socket: " << WSAGetLastError() << endl;
            // clean up and exit on error
            closesocket(this->currentSocket);
            WSACleanup();
            exit(-1);
        } 
            
        cout << "Waiting for second player to connect..." << endl;
    }

    // Start responding to broadcasts
    virtual void startRespondingForBroadcast() {
        this->responding = true;
        this->responseThread = std::thread(&PlayerServer::responseForBroadcast, this);
    }

    // Stop responding to broadcasts
    virtual void stopRespondingForBroadcast() {
        this->responding = false;
        this->responseThread.join();
    }
};

// Client 
class Client : public MySocket{
public:
    // Constructor address of server to connect to 
    Client(const char * addr) 
        : MySocket() {
        // Set service address and port
        this->service = sockaddr_in(); 
        this->service.sin_family = AF_INET;
        this->service.sin_addr.s_addr = inet_addr(addr); // Address of the server
        this->service.sin_port = htons(PORT); // 55555
        // Using the bind function
        if( connect( this->currentSocket, (SOCKADDR*)&( this->service ), sizeof( this->service ) ) == SOCKET_ERROR){
            cout << "bind() failed: " << WSAGetLastError() << endl;
            // Close socket and clean up on error
            closesocket(currentSocket);
            WSACleanup();
            exit(-1);
        }

        cout << "Connected to The server !" << endl;
    }
};

// Accepted enemy
class EnemySocket {
    SOCKET currSocket;
public:
    EnemySocket(const SOCKET & ownSocket) {
        // Accept connection
        this->currSocket = accept( ownSocket, NULL, NULL );
        if(this->currSocket == INVALID_SOCKET){
            cout << "accept failed: " << WSAGetLastError() << endl;
            // On error clean up and exit
            WSACleanup();
            exit(-1);
        } else {
            cout << "Connected with player!" << endl;
        }
    }

    // Get socket
    const SOCKET & getSocket() {
        return this->currSocket;
    }  
};

// Send data to Socket
void send(const SOCKET & to, const string buffer) {
    int sbyteCount = send(to, buffer.c_str(), BUFFER_SIZE, 0);
    if(sbyteCount == SOCKET_ERROR){
        cout << "Send error: " << WSAGetLastError() << endl;
        exit(-1);
    } 

    cout << "Sent: " << sbyteCount << " bytes\n";
}

// Receive data from Socket
string receive(const SOCKET & from) {
    char receiveBuffer[BUFFER_SIZE];
    int rbyteCount = recv(from, receiveBuffer, BUFFER_SIZE, 0);
    if(rbyteCount < 0){
        cout << "Server recv error: " << WSAGetLastError() << endl;
        exit(-1);
    }

    receiveBuffer[rbyteCount] = '\0';

    return string(receiveBuffer);
}

enum Choice {
    Paper = 0b001,
    Scisors = 0b010,
    Rock = 0b011
};

// Translates enum to string
string translate(const Choice & choice) {
    switch (choice)
    {
    case Paper:
        return "Paper";
        break;

    case Scisors:
        return "Scisors";
        break;

    case Rock:
        return "Rock";
        break;
    }

    return "";
}

// -1 lost, 0 draw, 1 win
int checkWin(const Choice & p1, const Choice & p2) {
    if ( p1 == Scisors && p2 == Paper ) 
        return 1;
    
    if ( p1 == Paper && p2 == Rock ) 
        return 1;

    if ( p1 == Rock && p2 == Scisors )
        return 1;

    if ( p1 == p2 )
        return 0;

    return -1;
}

int main(){
    // setup info
    bool set = false;
    bool isServer = false;

    // Player stuff
    string myNick = "";
    string enemyNick = "";

    // Get nick of player from user
    cout << "Give nick: ";
    myNick = getValue<string>();

    // Connection stuff
    MySocket * currSocket = nullptr;
    EnemySocket * enemySocket;

    while( !set ) {
        system("cls");
        cout << "1.Create game\n";
        cout << "2.Join game\n";
        switch( getValue<char>() ) {
            case '1': {
                // Create server
                currSocket = new PlayerServer(myNick);
                // Start listening for connections
                currSocket->startListening();
                // Go Online
                currSocket->startRespondingForBroadcast();

                // Create accepted enemy | wait for connection
                enemySocket = new EnemySocket( currSocket->getSocket() );
                // Send my nick to enemy
                send( enemySocket->getSocket(), myNick );
                
                // After connecting with player stop responding for broadcast
                currSocket->stopRespondingForBroadcast();

                // Set isServer to trues
                set = true;
                isServer = true;
            } break;

            case '2': {
                sockaddr_in serverAddr {};
                {
                    BroadcastSocket bSocket;
                    std::vector<std::pair<std::string, sockaddr_in>> discoveredServers;
                    do {
                        system("cls");
                        cout << "Searching for servers...\n";
                        // Start broadcast
                        bSocket.startBroadcast();    

                        // Get results
                        discoveredServers = bSocket.getResults();

                        // Print results
                        std::cout << "Servers found: " << discoveredServers.size() << std::endl;
                        if ( discoveredServers.size() == 0 )
                            std::cout << "No servers found." << std::endl;
                        else {
                            for( int i = 0; i < discoveredServers.size(); i++ ) {
                                std::cout << i + 1 << ": " << discoveredServers[i].first << std::endl;
                            }   
                        }

                        // ask for confirmation
                        std::cout << "\nSearch again? (y/n) ";
                    } while( tolower( getValue<char>() ) != 'n' );

                    system("cls");
                    // If no server to choose from, exit
                    if ( discoveredServers.size() == 0 ) {
                        std::cout << "No servers found." << std::endl;
                        system("pause");
                        break;
                    }

                    // Choose server
                    std::cout << "Select server id: \n";
                    // Print servers
                    for( int i = 0; i < discoveredServers.size(); i++ ) {
                        std::cout << i + 1 << ": " << discoveredServers[i].first << std::endl;
                    } 

                    // Get choice
                    int choice = getValue<int>();
                    while ( choice < 1 && choice > discoveredServers.size() ) {
                        std::cerr << "Wrong input _>";
                        choice = getValue<int>();
                    }

                    // Set server
                    serverAddr = discoveredServers[choice - 1].second;
                }

                // Create client socket which connects to the server
                currSocket = new Client( inet_ntoa( serverAddr.sin_addr ) );

                // When client you are accepted so sending as accepted socket
                send( currSocket->getSocket(), myNick );

                set = true;
                isServer = false;
            }
        }
    }

    // Get enemy nick
    if( !isServer ) {
        // Client
        enemyNick = receive( currSocket->getSocket() );
        cout << "You play against: " << enemyNick;
    } else {
        // Server
        enemyNick = receive( enemySocket->getSocket() );
        cout << "You play against: " << enemyNick;
    }

    // Required variables
    bool running = true;
    bool goodChoice = false;
    Choice currUserChoice, enemyChoice;
    int currWins = 0, enemyWins = 0;
    string choiceGlobal;

    while( running ) {
        system("cls");
        goodChoice = false;
        choiceGlobal = "";

        cout << "Wynik: " << myNick << "(" << currWins << ") - (" << enemyWins << ")" << enemyNick << "\n";

        do {
            cout << "-\n1.Paper\n2.Scisors\n3.Rock\n";
            choiceGlobal = getValue<string>();

            int intChoice = atoi(choiceGlobal.c_str());
            if ( intChoice >= 1 && intChoice <= 3) {
                goodChoice = true;
                currUserChoice = static_cast<Choice>(intChoice);
            }
        } while(!goodChoice);

        if (isServer) {
            send(enemySocket->getSocket(), choiceGlobal);
            Sleep(500);
            string choice = receive(enemySocket->getSocket());
            enemyChoice = static_cast<Choice>(atoi(choice.c_str()));
        } else {
            cout << "Waiting for other player to chose !\n";
            string choice = receive(currSocket->getSocket());
            enemyChoice = static_cast<Choice>(atoi(choice.c_str()));
            send(currSocket->getSocket(), choiceGlobal);
        }

        cout << translate(currUserChoice) << " VS " << translate(enemyChoice);
        // -1 lost, 0 draw, 1 win
        int status = checkWin(currUserChoice, enemyChoice);
        if ( status == -1 ) {
            enemyWins++;
            cout << " - LOST";
        } else if ( status == 1 ) {
            currWins++;
            cout << " - WIN";
        } else {
            cout << " - DRAW";
        }

        cout << "\nKliknij dowolny klawisz by kontynuowac";
        system("pause>nul");
    }
}