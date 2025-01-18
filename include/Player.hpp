#pragma once

#include <Config.hpp>
#include <Socket.hpp>

#include <sprites/Sprite.hpp>

class Choice {
public:
    enum Value {
        Paper = 1,
        Scissors = 2,
        Rock = 0
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

class Player : public Sprite {
protected:
    int score;
    std::string name;
    Choice::Value choice;
    std::atomic<bool> chosen = false;
public:
    // Constructor
    // 1. Name of the player
    // 2. Connection socket
    Player(std::string name, Sprite sprite);

    // Choose by other players object
    virtual void choose() {};
    virtual void choose(Choice::Value choice);

    void resetChosen() { this->chosen = false; this->sFrame = 0; }
    bool hasChosen() { return this->chosen.operator bool(); }
    
    void showResult();

    // Getters
    const Choice::Value & getChoice();
    const std::string & getName();
    const int & getScore();

    // add score
    void addScore();

    void draw() override;
    void update(float deltaTime) override;
};

class OnlinePlayer : public Player {
protected:
    std::shared_ptr<MySocket> connectionSocket;
public:
    // Constructor
    // 1. Name of the player
    // 2. Connection socket
    OnlinePlayer(std::string name, std::shared_ptr<MySocket> connectionSocket, Sprite sprite);

    // Choose
    virtual void choose() {};
    void choose(Choice::Value choice); 
};

class Enemy : public OnlinePlayer {
public:
    Enemy(std::shared_ptr<MySocket> connectionSocket, Sprite sprite);

    // Choose
    // It receive choice from player
    virtual void choose();
};

class Computer : public Player {
public:
    Computer(Sprite sprite);

    // Choose
    virtual void choose();
};