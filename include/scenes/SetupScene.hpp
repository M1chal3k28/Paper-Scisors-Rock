#pragma once

#include <scenes/Scene.hpp>
#include <ResourceManager.hpp>
#include <Background.hpp>
#include <Button.hpp>
#include <InputField.hpp>

#include <Game.hpp>

class SetupScene : public Scene {
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
public:
    SetupScene();
    ~SetupScene();

    void prepareResources() override;
    void update(float deltaTime) override;
    void draw() const override;
    void cleanUp() override;
};