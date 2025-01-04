#pragma once

#include <Config.hpp>
#include <Player.hpp>
#include <Sockets.hpp>

#include <scenes/SceneManager.hpp>

#define GAME Game::getInstance()

class Game {
    // Private constructor for singleton pattern
    Game() {}
    
    // Deleted copy constructor and assignment operator
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    // Players
    std::unique_ptr<Player> player = nullptr;
    std::unique_ptr<Player> enemy = nullptr;

        // Connection stuff
    // This is either server socket or client socket
    // Depending on needing
    std::shared_ptr<MySocket> serverOrClientSocket = nullptr;
    // This is always second player socket
    // It's by accepting connection by server
    std::shared_ptr<MySocket> enemySocket = nullptr;

    // Setup thread
    // To isolate update/draw logic from server/client setup
    std::future<void> setupThread;
    bool isSetUp = false;
    bool finishedSetup = false;

    // Type of player
    PlayerType::Value playerType; // update logic depend on this
    PlayerType::Value enemyType;

    // has server error ?
    std::atomic<bool> serverError = false;
public:
    ~Game();
    // Singleton access to game instance
    static Game& getInstance();

    // -1 lost, 0 draw, 1 win 
    // checks if p1 won or lost against p2
    int checkWin( const Choice::Value &p1, const Choice::Value &p2 );

    // Set up game
    // 1. Get nick name
    // 2. Set player type
    // 3. Setup server/client socket
    void setup(std::string nick, PlayerType::Value type);

        //  Set up server socket
    // 1. Create server socket
    // 2. Start listening for connections
    // 3. Go Online ( respond for broadcasts )
    // 4. Create accepted socket | wait for connection
    // 5. Send my nick to enemy
    // 6. After connecting with player stop responding for broadcast
    // 7. Create enemy and player object
    void setupServer( const std::string& nick );

        // Setup client
    // 1. Get all servers online
    // 2. Select server
    // 3. Stop broadcasting
    // 4. Connect to selected server
    // 5. Send my nick to host player aka server
    // 6. Create enemy and player object based on connection socket
    void setupClient( const std::string& nick );

        // Setup offline
    // 1. Create player object
    void setupOffline( const std::string& nick );

    // Main loop of the game
    void run();
    
    // Deinitialize game
    // 1. Stop listening for connections
    // 2. Close sockets
    // 3. Join setup thread
    void deinitialize();

    // return whether server has error
    bool isServerError();
    bool isSetupFinished();

    PlayerType::Value getPlayerType();

    std::string getNick(PlayerType::Value type);

    std::unique_ptr<Player> movePlayer() {
        return std::move(this->player);
    }

    std::unique_ptr<Player> moveEnemy() {
        return std::move(this->enemy);
    }
};
