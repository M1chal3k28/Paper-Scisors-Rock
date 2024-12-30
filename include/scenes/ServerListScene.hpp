#pragma once 

#include <Config.hpp>
#include <scenes/Scene.hpp>
#include <Background.hpp>
#include <sprites/Button.hpp>
#include <sprites/TextButton.hpp>
#include <Text.hpp>

#include <Game.hpp>

#include <sockets/BroadcastSocket.hpp>

class ServerListScene : public Scene {
    // Socket for broadcast
    BroadcastSocket broadcastSocket;

    // Copy of discovered servers
    std::vector<std::pair<std::string, sockaddr_in>> discoveredServers;

    // server buttons
    std::vector<std::unique_ptr<TextButton>> serverButtons;
    std::vector<std::vector<std::unique_ptr<TextButton>>> pages;
    size_t currentPage = 0;

    std::unique_ptr<Text> info;
public:
    ServerListScene();
    ~ServerListScene();
    void prepareResources() override;
    void update(float deltaTime) override;
    void draw() const override;
    void cleanUp() override;
};