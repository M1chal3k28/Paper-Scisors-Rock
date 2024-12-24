#pragma once

#include <Scene.hpp>
#include <ResourceManager.hpp>

class IntroScene : public Scene {
public:
    void prepareResources() override;
    void update(float deltaTime) override;
    void draw() const override;
    void cleanUp() override;
};