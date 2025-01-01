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
    // Paging
    std::vector<std::vector<std::unique_ptr<TextButton>>> pages;
    size_t currentPage = 0;
    std::unique_ptr<Button> leftArrow = nullptr;
    std::unique_ptr<Button> rightArrow = nullptr;
    
    std::unique_ptr<Text> info = nullptr;

    // Serch for servers using broadcastSocket
    std::future<void> getServersThread;
    std::atomic<bool> isFinished = true;
    void getAvailableServers();

    // Back button
    std::unique_ptr<TextButton> backButton = nullptr;
public:
    ServerListScene();
    ~ServerListScene();
    void prepareResources() override;
    void update(float deltaTime) override;
    void draw() const override;
    void cleanUp() override;
};