#pragma once

#include <Config.hpp>
#include <Socket.hpp>

class Choice {
public:
    enum Value {
        Paper = 0b001,
        Scissors = 0b010,
        Rock = 0b011
    };

    static std::string toString(Choice::Value choice) {
        switch (choice)
        {
        case Choice::Paper:
            return "Paper";
            break;

        case Choice::Scissors:
            return "scissors";
            break;

        case Choice::Rock:
            return "Rock";    
            break; 
        }

        return "";
    }
};

class Player {
protected:
    int score;
    std::string name;
    Choice::Value choice;
    std::shared_ptr<MySocket> connectionSocket;
public:
    Player(std::string name, std::shared_ptr<MySocket> connectionSocket);

    virtual void choose();
    
    const Choice::Value & getChoice();
    const std::string & getName();
    const int & getScore();

    void addScore();
};

class Enemy : public Player {
public:
    Enemy(std::shared_ptr<MySocket> connectionSocket);

    virtual void choose();
};
