#include <scenes/HostLobbyScene.hpp>

HostLobbyScene::HostLobbyScene() {
    this->prepareResources();
}

HostLobbyScene::~HostLobbyScene() {
    this->cleanUp();
}

void HostLobbyScene::prepareResources() {
    // Init background if not done already
    M_BG;
}

void HostLobbyScene::update(float deltaTime) {
    M_BG.update(deltaTime);
}

void HostLobbyScene::draw() const {
    M_BG.draw();
}

void HostLobbyScene::cleanUp() {}