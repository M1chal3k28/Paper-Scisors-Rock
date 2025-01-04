#pragma once 

#include <Config.hpp>
#include <scenes/Scene.hpp>

#include <Game.hpp>
#include <scenes/Scenes.hpp>

class GameLoadingScene : public Scene {
    std::unique_ptr<Text> loadingText = nullptr;
public:
    void update( float deltaTime ) override;
    void draw() const override;
    void cleanUp() override;
    ~GameLoadingScene();
    GameLoadingScene();
    void prepareResources() override;
};