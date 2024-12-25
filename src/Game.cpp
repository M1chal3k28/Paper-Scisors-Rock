#include <Game.hpp>

// Singleton static instance
Game& Game::getInstance() {
    static Game instance;
    return instance;
}

PlayerType::Value Game::menu() {
    bool set = false;
    std::string temporary;

    while( !set ) {
        system("cls");

        cout << "1.Create game\n";
        cout << "2.Join game\n";
        getline(cin, temporary);

        switch( temporary[0] ) {
            case '1': return PlayerType::Host;
            case '2': return PlayerType::Client;
        }
    }

    exit(-1);
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
// 2. Setup server/client socket
void Game::setup() {
    string temporary;
    // Get nick from user
    cout << "Give nick: ";
    std::getline( cin, temporary );


    // Menu 
    this->playerType = this->menu();

    switch( this->playerType ) {
        case PlayerType::Host:
            this->setupServer( temporary );
        break;

        case PlayerType::Client:
            this->setupClient( temporary );
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
    // Create server
    this->serverOrClientSocket = std::make_unique<PlayerServer>(nick);
    // Start listening for connections
    this->serverOrClientSocket->startListening();
    // Go Online
    this->serverOrClientSocket->startRespondingForBroadcast();

    // Create accepted socket | wait for connection
    this->enemySocket = std::make_unique<EnemySocket>( this->serverOrClientSocket->getSocket() );
    // Send my nick to enemy
    this->enemySocket->_send( nick.c_str() );
    
    // After connecting with player stop responding for broadcast
    this->serverOrClientSocket->stopRespondingForBroadcast();

    // Enemy
    this->enemy = std::make_unique<Enemy>(enemySocket);

    // Player
    // It's connected to server so player sends to it
    this->player = std::make_unique<Player>(nick, this->enemySocket);
}

    // Setup client
// 1. Get all servers online
// 2. Select server
// 3. Stop broadcasting
// 4. Connect to selected server
// 5. Send my nick to host player aka server
// 6. Create enemy and player object based on connection socket
void Game::setupClient( const std::string& nick ) {
    // Adress of server to connect
    sockaddr_in serverAddr {};
    {
        // Broadcast for servers
        BroadcastSocket bSocket;
        // Servers found list
        std::vector<std::pair<std::string, sockaddr_in>> discoveredServers;

        // temporary string for getting user input
        std::string temporary;
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
            getline(cin, temporary);
        } while( tolower( temporary[0] ) != 'n' );

        system("cls");
        // If no server to choose from, exit
        if ( discoveredServers.size() == 0 ) {
            std::cout << "No servers found." << std::endl;
            system("pause");
            return;
        }

        // Choose server
        std::cout << "Select server id: \n";
        // Print servers
        for( int i = 0; i < discoveredServers.size(); i++ ) {
            std::cout << i + 1 << ": " << discoveredServers[i].first << std::endl;
        } 

        // Get choice
        getline(cin, temporary);
        int choice = atoi( temporary.c_str() );
        while ( choice < 1 && choice > discoveredServers.size() ) {
            std::cerr << "Wrong input _>";
            getline(cin, temporary);
            choice = atoi( temporary.c_str() );
        }

        // Set server
        serverAddr = discoveredServers[choice - 1].second;
    }

    // Create client socket which connects to the server
    this->serverOrClientSocket = std::make_unique<Client>( inet_ntoa( serverAddr.sin_addr ) );

    // Client is connected to server so sending to server
    this->serverOrClientSocket->_send( nick.c_str() );

    // Enemy
    this->enemy = std::make_unique<Enemy>( this->serverOrClientSocket );

    // Player 
    this->player = std::make_unique<Player>( nick, this->serverOrClientSocket );
}

void Game::run() {
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