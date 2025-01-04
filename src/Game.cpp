#include <Game.hpp>

#include <Background.hpp>

// Singleton static instance
Game& Game::getInstance() {
    static Game instance;
    return instance;
}

Game::~Game() {
    // Clear background sprites 
    // Just in case of crash
    M_BG.clear();

    this->deinitialize();
}

// -1 lost, 0 draw, 1 win
// checks if p1 won or lost against p2
int Game::checkWin( const Choice::Value & p1, const Choice::Value & p2 ) {
    if ( p1 == Choice::Scissors && p2 == Choice::Paper ) 
        return 1;
    
    if ( p1 == Choice::Paper && p2 == Choice::Rock ) 
        return 1;

    if ( p1 == Choice::Rock && p2 == Choice::Scissors )
        return 1;

    if ( p1 == p2 )
        return 0;

    return -1;
}

    // Set up game
// 1. Get nick name
// 2. Set player type
// 2. Setup server/client socket in separate thread
void Game::setup(std::string nick, PlayerType::Value type, sockaddr_in serverAddr) { 
    // Check if already set up
    if ( this->isSetUp ) return;

    // Set player type
    this->playerType = type;
    // Set flag
    this->isSetUp = true;
    // Switch player type
    switch( this->playerType ) {
        // Server
        case PlayerType::Host: 
            this->setupThread = std::async( std::launch::async, &Game::setupServer, this, nick );
        break;
        
        // Client
        case PlayerType::Client:
            this->setupThread = std::async( std::launch::async, &Game::setupClient, this, nick, serverAddr );
        break;

        // Offline
        case PlayerType::Offline:
            this->setupThread = std::async( std::launch::async, &Game::setupOffline, this, nick );
        break;
    }
}

    //  Set up server socket
// 1. Create server socket
// 2. Start listening for connections
// 3. Go Online ( respond for broadcasts )
// 4. Create accepted socket | wait for connection
// 5. Send my nick to enemy
// 6. After connecting with player stop responding for broadcast
// 7. Create enemy and player object
void Game::setupServer( const std::string& nick ) {
    try {
        // Create server
        this->serverOrClientSocket = std::make_unique<PlayerServer>(nick);
        // Start listening for connections
        this->serverOrClientSocket->startListening();
        // Go Online
        this->serverOrClientSocket->startRespondingForBroadcast();

        // Create accepted socket | wait for connection
        this->enemySocket = std::make_unique<EnemySocket>( );
        try {
            this->enemySocket->connectToServer( this->serverOrClientSocket->getSocket() );
            // Send my nick to enemy
            this->enemySocket->_send( nick.c_str() );
            
            // After connecting with player stop responding for broadcast
            this->serverOrClientSocket->stopRespondingForBroadcast();

            // Enemy
            this->enemy = std::make_unique<Enemy>(enemySocket, Sprite{
                PLAYER_TEXTURE_KEY,
                2,
                (Vector2)PLAYER_SIZE,
                (Vector2)PLAYER_LEFT_OFFSET
            });

            // Player
            // It's connected to server so player sends to it
            this->player = std::make_unique<OnlinePlayer>(nick, this->enemySocket, Sprite{
                PLAYER_TEXTURE_KEY,
                2,
                (Vector2)PLAYER_SIZE,
                (Vector2)PLAYER_RIGHT_OFFSET
            });

            // Finished setup
            this->finishedSetup = true;
        } catch(...) {
            // On error clean up and exit
            WSACleanup();
        }
    } catch( std::exception & e ) {
        // Handle exception
        MessageBox((HWND)GetWindowHandle(), e.what(), "Error", MB_OK | MB_ICONERROR);
        this->serverError = true;
        WSACleanup();
    }
}

    // Setup client
// 1. Get all servers online
// 2. Select server
// 3. Stop broadcasting
// 4. Connect to selected server
// 5. Send my nick to host player aka server
// 6. Create enemy and player object based on connection socket
void Game::setupClient( const std::string& nick, sockaddr_in serverAddr ) {
    // Adress of server to connect
    // sockaddr_in serverAddr {};
    // {
    //     // Broadcast for servers
    //     BroadcastSocket bSocket;
    //     // Servers found list
    //     std::vector<std::pair<std::string, sockaddr_in>> discoveredServers;

    //     // temporary string for getting user input
    //     std::string temporary;
    //     do {
    //         system("cls");
    //         cout << "Searching for servers...\n";
    //         // Start broadcast
    //         bSocket.startBroadcast();    

    //         // Get results
    //         discoveredServers = bSocket.getResults();

    //         // Print results
    //         std::cout << "Servers found: " << discoveredServers.size() << std::endl;
    //         if ( discoveredServers.size() == 0 )
    //             std::cout << "No servers found." << std::endl;
    //         else {
    //             for( int i = 0; i < discoveredServers.size(); i++ ) {
    //                 std::cout << i + 1 << ": " << discoveredServers[i].first << std::endl;
    //             }   
    //         }

    //         // ask for confirmation
    //         std::cout << "\nSearch again? (y/n) ";
    //         getline(cin, temporary);
    //     } while( tolower( temporary[0] ) != 'n' );

    //     system("cls");
    //     // If no server to choose from, exit
    //     if ( discoveredServers.size() == 0 ) {
    //         std::cout << "No servers found." << std::endl;
    //         system("pause");
    //         return;
    //     }

    //     // Choose server
    //     std::cout << "Select server id: \n";
    //     // Print servers
    //     for( int i = 0; i < discoveredServers.size(); i++ ) {
    //         std::cout << i + 1 << ": " << discoveredServers[i].first << std::endl;
    //     } 

    //     // Get choice
    //     getline(cin, temporary);
    //     int choice = atoi( temporary.c_str() );
    //     while ( choice < 1 && choice > discoveredServers.size() ) {
    //         std::cerr << "Wrong input _>";
    //         getline(cin, temporary);
    //         choice = atoi( temporary.c_str() );
    //     }

    //     // Set server
    //     serverAddr = discoveredServers[choice - 1].second;
    // }

    std::cout << inet_ntoa( serverAddr.sin_addr ) << "\n";
    try {
        // Create client socket which connects to the server
        this->serverOrClientSocket = std::make_unique<Client>( inet_ntoa( serverAddr.sin_addr ) );

        // Client is connected to server so sending to server
        // this->serverOrClientSocket->_send( nick.c_str() );

        // Enemy
        this->enemy = std::make_unique<Enemy>( this->serverOrClientSocket, Sprite{
            PLAYER_TEXTURE_KEY,
            2,
            (Vector2)PLAYER_SIZE,
            (Vector2)PLAYER_LEFT_OFFSET
        });

        // Player 
        this->player = std::make_unique<OnlinePlayer>( nick, this->serverOrClientSocket, Sprite{
            PLAYER_TEXTURE_KEY,
            2,
            (Vector2)PLAYER_SIZE,
            (Vector2)PLAYER_RIGHT_OFFSET
        });

        this->finishedSetup = true;
    } catch( std::exception &e ) {
        MessageBox((HWND)GetWindowHandle(), e.what(), "Error", MB_OK | MB_ICONERROR );
        WSACleanup();
        this->connectionError = true;
    }
}

void Game::setupOffline(const std::string &nick) {
    this->enemyType = PlayerType::Offline;
    this->playerType = PlayerType::Offline;
    this->player = std::make_unique<Player>(nick, Sprite{
        PLAYER_TEXTURE_KEY,
        2,
        (Vector2)PLAYER_SIZE,
        (Vector2)PLAYER_RIGHT_OFFSET
    });
    this->enemy = std::make_unique<Computer>(Sprite{
        PLAYER_TEXTURE_KEY,
        2,
        (Vector2)PLAYER_SIZE,
        (Vector2)PLAYER_LEFT_OFFSET
    });

    this->finishedSetup = true;
}

void Game::run()
{
    // Game loop
    while( true ) {
        system("cls");

        // Print game status
        cout << "Score: " << player->getName() << "(" << player->getScore() << ") - (" << enemy->getScore() << ")" << enemy->getName() << "\n";

            // Choose
        // Player chooses
        player->choose();
        // Enemy choice is received
        enemy->choose();

        // Show choices
        cout << Choice::toString( player->getChoice() ) << " VS " << Choice::toString( enemy->getChoice() );

        // Check win
        int win = checkWin( player->getChoice(), enemy->getChoice() );
        switch( win ) {
            case 1: {
                // Player won
                cout << "\nYou won!\n";
                player->addScore();
                break;
            }
            case 0: {
                // Draw
                cout << "\nDraw!\n";
                break;
            }
            case -1: {
                // Player lost
                cout << "\nYou lost!\n";
                enemy->addScore();
                break;
            }
        }

        system("pause");
    }
}

// Deinitialize game
// 1. Change flag
// 2. Close sockets
// 3. Delete objects
// ! it does not delete game object itself
void Game::deinitialize() {
    // Check if game is set up
    if ( !this->isSetUp ) return;

    // un set flag that game is set up
    this->isSetUp = false;
    this->serverError = false;
    this->connectionError = false;

    // Shutdown server first
    // This is gonna terminate accept function which is blocking game thread 
    // This function closes sockets
    // And also stops thread which is responding to broadcasts
    if(this->serverOrClientSocket)
        this->serverOrClientSocket->shutdownServer();

    // Join setup theard where server waits for connection
    if(this->setupThread.valid()) 
        this->setupThread.get();

    // Reset important objects
    // ----------------------------------------
    // This are unique ptr so it is better to release
        if(this->enemy)
            this->enemy.release();

        if(this->player)
            this->player.release();
    // This are shared ptr so it is better to reset
        if(this->enemySocket)
            this->enemySocket.reset();
        
        if(this->serverOrClientSocket)
            this->serverOrClientSocket.reset();
    // ----------------------------------------
}

bool Game::isServerError() {
    return this->serverError.operator bool();
}

bool Game::isSetupFinished() {
    return this->finishedSetup;
}

PlayerType::Value Game::getPlayerType() {
    return this->playerType;
}

bool Game::isConnectionError() {
    return this->connectionError.operator bool();
}

// std::string Game::getNick(PlayerType::Value type)
// {
//     switch(type) {
//         case PlayerType::Host:
//         case PlayerType::Client:
//         case PlayerType::Offline:
//             return this->player->getName();
            
//         case PlayerType::Enemy:
//             return this->enemy->getName();
//     }
//     return std::string();
// }
