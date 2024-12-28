#pragma once

#include <Config.hpp>
#include <scenes/Scene.hpp>

#include <Background.hpp>
#include <Button.hpp>

class HostLobbyScene : public Scene {
    // Button to return to main menu
    std::unique_ptr<Button> backButton = nullptr; 
public:
    HostLobbyScene();
    ~HostLobbyScene();
    void prepareResources() override;
    void update(float deltaTime) override;
    void draw() const override;
    void cleanUp() override;
};