#include <Sockets.hpp>
#include <Config.hpp>
#include <Player.hpp>

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

// -1 lost, 0 draw, 1 win
int checkWin(const Choice::Value & p1, const Choice::Value & p2) {
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
    std::unique_ptr<MySocket> currSocket = nullptr;
    std::unique_ptr<MySocket> enemySocket = nullptr;
    // For client to be able to send to server
    sockaddr_in serverAddr {};

    // Player 
    std::unique_ptr<Player> player = nullptr;
    std::unique_ptr<Enemy> enemy = nullptr;

    while( !set ) {
        system("cls");
        cout << "1.Create game\n";
        cout << "2.Join game\n";
        switch( getValue<char>() ) {
            case '1': {
                // Create server
                currSocket = std::make_unique<PlayerServer>(myNick);
                // Start listening for connections
                currSocket->startListening();
                // Go Online
                currSocket->startRespondingForBroadcast();

                // Create accepted enemy | wait for connection
                enemySocket = std::make_unique<EnemySocket>( currSocket->getSocket() );
                // Send my nick to enemy
                enemySocket->_send( myNick.c_str() );
                
                // After connecting with player stop responding for broadcast
                currSocket->stopRespondingForBroadcast();

                // Set isServer to trues
                set = true;
                isServer = true;

                // Player 
                player = std::make_unique<Player>(myNick, enemySocket);
                // Enemy
                enemy = std::make_unique<Enemy>(enemySocket);
            } break;

            case '2': {
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
                currSocket = std::make_unique<Client>( inet_ntoa( serverAddr.sin_addr ) );

                // Client is connected to server so sending to server
                currSocket->_send( myNick.c_str() );

                // set isServer to false
                set = true;
                isServer = false;

                // Player 
                player = std::make_unique<Player>(myNick, currSocket);
                // Enemy
                enemy = std::make_unique<Enemy>(currSocket);
            }
        }
    }

    // Required variables
    bool running = true;
    bool goodChoice = false;
    Choice currUserChoice, enemyChoice;
    int currWins = 0, enemyWins = 0;
    string choiceGlobal;

    while( running ) {
        system("cls");
        // Reset
        goodChoice = false;
        choiceGlobal = "";

        // Print scores
        cout << "Wynik: " << player->getName() << "(" << player->getScore() << ") - (" << enemy->getScore() << ")" << enemy->getName() << "\n";

        // Choose
        player->choose();
        enemy->choose();

        // Print choices
        cout << Choice::toString(player->getChoice()) << " VS " << Choice::toString(enemy->getChoice());
        // -1 lost, 0 draw, 1 win
        // Check win and print result
        int status = checkWin(player->getChoice(), enemy->getChoice());
        if ( status == -1 ) {
            enemy->addScore();
            cout << " - LOST";
        } else if ( status == 1 ) {
            player->addScore();
            cout << " - WIN";
        } else {
            cout << " - DRAW";
        }

        cout << "\nKliknij dowolny klawisz by kontynuowac";
        system("pause>nul");
    }
}