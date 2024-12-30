#pragma once

#include <scenes/Scene.hpp>
#include <ResourceManager.hpp>
#include <Background.hpp>
#include <sprites/Button.hpp>
#include <sprites/TextButton.hpp>
#include <InputField.hpp>
#include <Text.hpp>

#include <Game.hpp>

class SetupScene : public Scene {
    // Exit button
    std::unique_ptr<Button> exitButton = nullptr;

    // First step of the setup scene
    std::string nick;
    std::unique_ptr<Button> confirmButton = nullptr;
    std::unique_ptr<InputField> nameInput = nullptr;
    bool nickGiven = false;

    // Second step of the setup scene
    PlayerType::Value playerType;
    std::unique_ptr<Button> hostButton = nullptr;
    std::unique_ptr<Button> clientButton = nullptr;
    std::unique_ptr<Button> backButton = nullptr;
    std::unique_ptr<Button> offlineButton = nullptr;
    bool playerTypeGiven = false;

    // Game title text
    std::unique_ptr<Text> title = nullptr;
public:
    SetupScene();
    ~SetupScene();

    void prepareResources() override;
    void update(float deltaTime) override;
    void draw() const override;
    void cleanUp() override;
};