#pragma once

#include <Config.hpp>
#include <scenes/Scene.hpp>
#include <Background.hpp>
#include <sprites/Button.hpp>
#include <ResourceManager.hpp>
#include <Text.hpp>
#include <Game.hpp>
class GameScene : public Scene
{
    // Player related stuff
    std::unique_ptr<Sprite> playerSprite = nullptr;
    std::unique_ptr<Sprite> enemySprite = nullptr;
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
public:
    GameScene();
    ~GameScene();
    void prepareResources() override;
    void update(float deltaTime) override;
    void draw() const override;
    void cleanUp() override;
};