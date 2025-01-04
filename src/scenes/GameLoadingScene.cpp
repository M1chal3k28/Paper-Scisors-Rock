#include <scenes/GameLoadingScene.hpp>

GameLoadingScene::GameLoadingScene() {
    this->prepareResources();

    // Loading text
    this->loadingText = std::make_unique<Text>(
        "Loading...",
        Vector2{0, 0},
        Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2},
        MINECRAFT_FONT_KEY,
        TITLE_SIZE,
        TEXT_SPACING,
        MY_ORANGE
    );

    this->loadingText->tCenterOffset();
}

GameLoadingScene::~GameLoadingScene() {
    this->cleanUp();
}

void GameLoadingScene::cleanUp() {}

void GameLoadingScene::prepareResources() {
    // Init background if not done already
    M_BG;

    // Ensure that the font is loaded
    RESOURCE_MANAGER.loadFont(MINECRAFT_FONT_KEY, "assets/Minecraft.ttf");
}

void GameLoadingScene::update(float deltaTime) {
    M_BG.update(deltaTime);
    if (GAME.isSetupFinished()) {
        SCENE_MANAGER.popScene();
        SCENE_MANAGER.pushScene(SceneType::GAME_SCENE);
    }

    if (GAME.isConnectionError()) {
        SCENE_MANAGER.popScene();
        GAME.deinitialize();
    }
}

void GameLoadingScene::draw() const {
    M_BG.draw();
    this->loadingText->draw();
}