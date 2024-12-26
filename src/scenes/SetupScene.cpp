#include <scenes/SetupScene.hpp>

SetupScene::SetupScene() {
    // This will create background object 
    // Load button png sprite sheet
    this->prepareResources();

    // Then you can create button 
    this->confirmButton = std::make_unique<Button>(
        Vector2{ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 + 200 }, 
        (Vector2)MENU_BUTTON_SIZE, 
        "Confirm", 
        RESOURCE_MANAGER.getTexture("button"), 
        RESOURCE_MANAGER.getFont("font-roboto-regular"),
        []() {}
    );
}

SetupScene::~SetupScene() {
    this->cleanUp();
}

void SetupScene::prepareResources() {
    // This will create background object if it doesn't exist
    M_BG;
    // Load button png sprite sheet
    RESOURCE_MANAGER.loadTexture("button", "assets/menuButton.png");
    // Load font
    RESOURCE_MANAGER.loadFont("font-roboto-regular", "assets/Roboto-Regular.ttf");
}

void SetupScene::update(float deltaTime) {
    M_BG.update(deltaTime);
    // Update button
    this->confirmButton->update();
}

void SetupScene::draw() const {
    M_BG.draw();
    this->confirmButton->draw();
}

void SetupScene::cleanUp() {}
