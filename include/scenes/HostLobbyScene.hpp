#pragma once

#include <Config.hpp>
#include <scenes/Scene.hpp>

#include <Background.hpp>
#include <sprites/Button.hpp>
#include <sprites/TextButton.hpp>

class HostLobbyScene : public Scene {
    // Button to return to main menu
    std::unique_ptr<Button> backButton = nullptr; 

    // Text logic
    Vector2 infoPosition;
    std::string infoText = "Waiting For Players";
    Vector2 infoOffset;
public:
    HostLobbyScene();
    ~HostLobbyScene();
    void prepareResources() override;
    void update(float deltaTime) override;
    void draw() const override;
    void cleanUp() override;
};