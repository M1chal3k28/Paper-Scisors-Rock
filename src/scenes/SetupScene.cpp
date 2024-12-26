#include <scenes/SetupScene.hpp>

SetupScene::SetupScene() {
    this->prepareResources();
}

SetupScene::~SetupScene() {
    this->cleanUp();
}

void SetupScene::prepareResources() {
    // This will create bg object if it doesn't exist
    M_BG;
}

void SetupScene::update(float deltaTime) {
    M_BG.update(deltaTime);
}

void SetupScene::draw() const {
    M_BG.draw();
}

void SetupScene::cleanUp() {}
