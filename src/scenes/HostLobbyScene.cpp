#include <scenes/HostLobbyScene.hpp>
#include <scenes/SceneManager.hpp>

HostLobbyScene::HostLobbyScene() {
    this->prepareResources();
    
    // Set text info offset and pos
    Vector2 measurement = MeasureTextEx(*RESOURCE_MANAGER.getFont(MINECRAFT_FONT_KEY), this->infoText.c_str(), TITLE_SIZE, TEXT_SPACING);
    this->infoOffset = Vector2{ measurement.x / 2.f, measurement.y / 2.f };
    this->infoPosition = Vector2{ (float)GetScreenWidth() / 2, 200 };

    // Create back button
    this->backButton = std::make_unique<TextButton>(
        Vector2{ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 + 345 }, 
        (Vector2)MENU_BUTTON_SIZE, 
        (Vector2)MENU_BUTTON_SHEET_OFFSET,
        "Back", 
        BUTTON_TXT_KEY, 
        MINECRAFT_FONT_KEY,
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
    RESOURCE_MANAGER.loadTexture(BUTTON_TXT_KEY, "assets/buttons.png");
    // Load font
    RESOURCE_MANAGER.loadFont(MINECRAFT_FONT_KEY, "assets/Minecraft.ttf");
}

void HostLobbyScene::update(float deltaTime) {
    M_BG.update(deltaTime);

    // Set cursor to default
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    this->backButton->update(deltaTime);

    // Check if server is running
    if (GAME.isServerError()) {
        SCENE_MANAGER.popScene();
        GAME.deinitialize();
    }

    if (GAME.isSetupFinished()) {
        SCENE_MANAGER.popScene();
        SCENE_MANAGER.pushScene(SceneType::LOADING_SCENE);
        return;
    }
}

void HostLobbyScene::draw() const {
    M_BG.draw();

    // Draw text that server wait for client
    DrawTextPro(
        *RESOURCE_MANAGER.getFont(MINECRAFT_FONT_KEY),
        this->infoText.c_str(),
        this->infoPosition,
        this->infoOffset,
        0.f,
        TITLE_SIZE,
        TEXT_SPACING,
        MY_ORANGE
    );

    this->backButton->draw();
}

void HostLobbyScene::cleanUp() {}