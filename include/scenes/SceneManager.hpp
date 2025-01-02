#pragma once 

#include <Config.hpp>
#include <scenes/Scenes.hpp>

#define SCENE_MANAGER SceneManager::getInstance()

class SceneManager {
    std::stack<std::unique_ptr<Scene>> scenesStack;
public:
    static SceneManager& getInstance();

    ~SceneManager();

    void pushScene( const SceneType::Value& scene );
    void pushScene(  std::unique_ptr<Scene> scene ); 

    void popScene();

    void update( float deltaTime );

    void draw() const;
};