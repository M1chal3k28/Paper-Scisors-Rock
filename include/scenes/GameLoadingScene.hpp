#pragma once 

#include <Config.hpp>
#include <scenes/Scene.hpp>

#include <Game.hpp>
#include <scenes/Scenes.hpp>

#define LOADING_SCENE_DURATION 0.5f

class GameLoadingScene : public Scene {
    std::unique_ptr<Text> loadingText = nullptr;
    float timer = 0.0f;
public:
    void update( float deltaTime ) override;
    void draw() const override;
    void cleanUp() override;
    ~GameLoadingScene();
    GameLoadingScene();
    void prepareResources() override;
};