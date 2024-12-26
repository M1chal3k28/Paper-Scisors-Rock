#pragma once

#include <scenes/Scene.hpp>
#include <ResourceManager.hpp>
#include <Background.hpp>

#include <Game.hpp>

class SetupScene : public Scene {
    std::string nick;
    PlayerType::Value playerType;

public:
    SetupScene();
    ~SetupScene();

    void prepareResources() override;
    void update(float deltaTime) override;
    void draw() const override;
    void cleanUp() override;
};