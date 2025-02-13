#pragma once

#include <Config.hpp>
#include <scenes/Scene.hpp>
#include <Background.hpp>
#include <sprites/Button.hpp>
#include <ResourceManager.hpp>
#include <Text.hpp>
#include <Game.hpp>

#define RESULTS_OFFSET 4.f
#define NEW_ROUND_OFFSET 2.f

class GameScene : public Scene
{
    float resultsTimer = RESULTS_OFFSET; // Timer goes down
    float newRoundTimer = NEW_ROUND_OFFSET; // Timer goes up
    std::unique_ptr<Text> infoText = nullptr;
    std::atomic<bool> showedResults = false;
    // Player related stuff
    
    // Moved from game which setup them
    std::unique_ptr<Player> player = nullptr;
    std::unique_ptr<Player> enemy = nullptr;

    std::unique_ptr<Text> playerName = nullptr;
    std::unique_ptr<Text> enemyName = nullptr;
    std::unique_ptr<Text> playerScore = nullptr;
    std::unique_ptr<Text> enemyScore = nullptr;
    std::unique_ptr<Sprite> playerReady = nullptr;
    std::unique_ptr<Sprite> enemyReady = nullptr;

    // Buttons
    std::unique_ptr<Button> exitButton = nullptr;
    std::unique_ptr<Button> homeButton = nullptr;
    std::unique_ptr<Button> paperButton = nullptr;
    std::unique_ptr<Button> rockButton = nullptr;
    std::unique_ptr<Button> scissorsButton = nullptr;

    // logic stuff
    std::future<void> enemyChoiceThread;
    std::atomic<bool> fatalError = false;

    // Choose by player method
    void playerChoose(Choice::Value choice);
    void enemyChoose();
public:
    GameScene();
    // Reset choices, unlock buttons, reset states, etc
    // Starts new game round
    void newRound();
    ~GameScene();
    void prepareResources() override;
    void update(float deltaTime) override;
    void draw() const override;
    void cleanUp() override;
};