#pragma once

#include <Config.hpp>
#include <Socket.hpp>

class Choice {
public:
    enum Value {
        Paper = 1,
        Scissors = 2,
        Rock = 3
    };

    static std::string toString(Choice::Value choice) {
        switch (choice)
        {
        case Choice::Paper:
            return "Paper";
            break;

        case Choice::Scissors:
            return "Scissors";
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
public:
    // Constructor
    // 1. Name of the player
    // 2. Connection socket
    Player(std::string name);

    // Choose
    virtual void choose();
    
    // Getters
    const Choice::Value & getChoice();
    const std::string & getName();
    const int & getScore();

    // add score
    void addScore();
};

class OnlinePlayer : public Player {
protected:
    std::shared_ptr<MySocket> connectionSocket;
public:
    // Constructor
    // 1. Name of the player
    // 2. Connection socket
    OnlinePlayer(std::string name, std::shared_ptr<MySocket> connectionSocket);

    // Choose
    virtual void choose();
};

class Enemy : public OnlinePlayer {
public:
    Enemy(std::shared_ptr<MySocket> connectionSocket);

    // Choose
    // It receive choice from player
    virtual void choose();
};

class Computer : public Player {
public:
    Computer();

    // Choose
    virtual void choose();
};