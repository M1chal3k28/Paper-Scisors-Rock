#include <BroadcastSocket.hpp>

// Constructor
BroadcastSocket::BroadcastSocket() : running(false), discoveredServers() {
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
        // WSACleanup();
        return;
    }

    // Set timeout for data receiving (recvfrom)
    int timeout = TIMEOUT_MS;
    setsockopt(this->broadcastSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

    this->broadcastAddr = sockaddr_in();
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_addr.s_addr = INADDR_BROADCAST; // Broadcast to all addresses on the network on port PORT
    broadcastAddr.sin_port = htons(PORT); // Set port PORT

    // Set socket to broadcast
    BOOL broadcastEnable = TRUE;
    setsockopt(this->broadcastSocket, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastEnable, sizeof(broadcastEnable));
}

// Destructor Clears the socket
BroadcastSocket::~BroadcastSocket() {
    this->running = false;
    if (this->discoverThread.valid())
        this->discoverThread.get();
    
    closesocket(this->broadcastSocket);
    // WSACleanup();
}

// Starts the broadcast in a separate thread
void BroadcastSocket::startBroadcast() {
    this->running = true;
    this->discoverThread = std::async(std::launch::async, &BroadcastSocket::searchForServers, this);
}

void BroadcastSocket::searchOnce() {
    if (this->discoverThread.valid())
        this->discoverThread.get();

    this->running = false;
    this->discoverThread = std::async(std::launch::async, &BroadcastSocket::searchForServers, this);
}

// Stops the broadcast and the thread
std::vector<std::pair<std::string, sockaddr_in>> BroadcastSocket::getResults() {
    // Check if thread is running
    if ( !this->discoverThread.valid() ) {
        throw std::runtime_error("Thread not started, could not get results from BroadcastSocket");
        return {};
    }

    // Stop searching
    this->running = false;
    // Join thread
    this->discoverThread.get();
    return this->discoveredServers;
}

// Function for searching for servers
void BroadcastSocket::searchForServers() {
    // Buffer for receiving data
    char buffer[BUFFER_SIZE];
    // Address of sender (server)
    sockaddr_in serverAddr {};
    // Length of address
    int serverAddrLen = sizeof(serverAddr);

    // Search for servers while running
    do {
        this->discoveredServers.clear(); // Clear discovered servers

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
                this->discoveredServers.emplace_back(std::make_pair(std::string(buffer), serverAddr));
            } else {
                // No more data
                break;
            }
        }

        // Wait for 2 seconds before next search
        if ( this->running )
            std::this_thread::sleep_for(std::chrono::seconds(2));
    } while ( this->running );
}