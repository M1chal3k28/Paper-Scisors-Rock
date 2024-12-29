#include <scenes/HostLobbyScene.hpp>

#include <scenes/SceneManager.hpp>

HostLobbyScene::HostLobbyScene() {
    this->prepareResources();
    
    this->backButton = std::make_unique<Button>(
        Vector2{ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 + 345 }, 
        (Vector2)MENU_BUTTON_SIZE, 
        Vector2{0, 0},
        "Back", 
        "button", 
        "minecraft-font",
        [this]() {
            // This is a function that will be called when the button is clicked
            // Go back to main menu scene
            SCENE_MANAGER.popScene();

            // Deinitialize game 
            GAME.deinitialize();
        }
    );
}

HostLobbyScene::~HostLobbyScene() {
    this->cleanUp();
}

void HostLobbyScene::prepareResources() {
    // Init background if not done already
    M_BG;
    // Load button png sprite sheet
    RESOURCE_MANAGER.loadTexture("button", "assets/buttons.png");
    // Load font
    RESOURCE_MANAGER.loadFont("minecraft-font", "assets/Minecraft.ttf");
}

void HostLobbyScene::update(float deltaTime) {
    M_BG.update(deltaTime);

    // Set cursor to default
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    this->backButton->update();
}

void HostLobbyScene::draw() const {
    M_BG.draw();

    this->backButton->draw();
}

void HostLobbyScene::cleanUp() {}