#pragma once

#include <Scene.hpp>
#include <ResourceManager.hpp>

#define INTRO_SCENE_DURATION 5.f

class IntroScene : public Scene {
    float timer = 0.0f;
public:
    IntroScene();
    ~IntroScene();

    void prepareResources() override;
    void update(float deltaTime) override;
    void draw() const override;
    void cleanUp() override;
};