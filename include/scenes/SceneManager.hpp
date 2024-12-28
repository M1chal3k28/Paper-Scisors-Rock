#pragma once 

#include <Config.hpp>
#include <scenes/Scenes.hpp>

#define SCENE_MANAGER SceneManager::getInstance()

class SceneType {
public:
    enum Value {
        INTRO = 0,
        SETUP,
        HOST_LOBBY
    };    
};

class SceneManager {
    std::stack<std::unique_ptr<Scene>> scenesStack;
public:
    static SceneManager& getInstance();

    ~SceneManager();

    void pushScene( const SceneType::Value& scene );

    void popScene();

    void update( float deltaTime );

    void draw() const;
};