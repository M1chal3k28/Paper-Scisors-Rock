#pragma once

#include <Config.hpp>
#include <scenes/Scene.hpp>

#include <Background.hpp>

class HostLobbyScene : public Scene {
    public:
        HostLobbyScene();
        ~HostLobbyScene();
        void prepareResources() override;
        void update(float deltaTime) override;
        void draw() const override;
        void cleanUp() override;
};