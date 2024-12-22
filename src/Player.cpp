#include <Player.hpp>

Player::Player(std::string name, std::unique_ptr<MySocket>& enemySocket) 
    :  
    name(name),
    score(0), 
    choice(Choice::Paper), 
    // enemy socket is required to send the player's choice
    // it works cause player is server which accepted connection to enemy socket
    enemySocket( std::make_shared<MySocket>(*enemySocket) ) {}

void Player::choose() {
    bool goodChoice = false;
    // This is sent to the enemy
    string choiceString;
    do {
        cout << "-\n1.Paper\n2.scissors\n3.Rock\nYour choice(1-3): ";
        choiceString = "";
        getline(cin, choiceString);

        // Convert to int to check if it is between 1 and 3
        int intChoice = atoi(choiceString.c_str());
        if ( intChoice >= 1 && intChoice <= 3) {
            // Choice is good
            goodChoice = true;
            // Convert to Choice
            this->choice = static_cast<Choice::Value>(intChoice);
        }
    } while(!goodChoice);

    // Send choice to enemy
    this->enemySocket->_send( choiceString.c_str() );
}

const Choice::Value &Player::getChoice() {
    return this->choice;
}

const std::string &Player::getName() {
    return this->name;
}

const int &Player::getScore() {
    return this->score;
}

void Player::addScore() {
    this->score += 1;
}

Enemy::Enemy(std::unique_ptr<MySocket>& enemySocket)
    : Player("", enemySocket) {
        // Enemy's name is empty
        // receive name from enemy
        // it works cause enemy is client connected to server so it can receive data from server
        this->name = this->enemySocket->receive();
    }

void Enemy::choose() {
    // Enemy's choice is received from the player
    std::string choiceString = this->enemySocket->receive();
    // Convert to int to check if it is between 1 and 3
    this->choice = static_cast<Choice::Value>(atoi(choiceString.c_str()));
}
