#include <scenes/GameScene.hpp>
// #include "GameScene.hpp"

GameScene::~GameScene() {
    this->cleanUp();
}

GameScene::GameScene() {
    this->prepareResources();

    // Initialize the game scene's properties
    this->playerName = std::make_unique<Text>(
        GAME.getNick(GAME.getPlayerType()),
        Vector2{0, 0},
        Vector2{10, 10},
        MINECRAFT_FONT_KEY,
        TITLE_SIZE / 2,
        TEXT_SPACING,
        MY_ORANGE
    );

    std::string enemyType = GAME.getNick(PlayerType::Enemy);
    Vector2 measure = MeasureTextEx(*RESOURCE_MANAGER.getFont(MINECRAFT_FONT_KEY), enemyType.c_str(), TITLE_SIZE / 2, TEXT_SPACING);
    this->enemyName = std::make_unique<Text>(
        enemyType,
        Vector2{0, 0},
        Vector2{(float)GetScreenWidth() - 10 - measure.x,  (float)GetScreenHeight() - 10 - measure.y},
        MINECRAFT_FONT_KEY,
        TITLE_SIZE / 2,
        TEXT_SPACING,
        MY_ORANGE
    );

    this->homeButton = std::make_unique<Button>(
        Vector2{ GetScreenWidth() - (Vector2)SMALL_BUTTON_SIZE.x / 2 - 10, (Vector2)SMALL_BUTTON_SIZE.y / 2 + 10 },
        (Vector2)SMALL_BUTTON_SIZE,
        (Vector2)SMALL_BUTTON_HOME_OFFSET,
        BUTTON_TXT_KEY,
        [this]() {
            SCENE_MANAGER.popScene();

            // Deinitialize game 
            GAME.deinitialize();
        }
    );
}

void GameScene::prepareResources() {
    RESOURCE_MANAGER.loadTexture(BUTTON_TXT_KEY, "assets/button.png");
    RESOURCE_MANAGER.loadFont(MINECRAFT_FONT_KEY, "assets/Minecraft.ttf");
}

void GameScene::update(float deltaTime) {
    // Update the game scene's properties
    M_BG.update(deltaTime);

    this->homeButton->update(deltaTime);
}

void GameScene::draw() const {
    M_BG.draw();

    this->playerName->draw();
    this->enemyName->draw();
    this->homeButton->draw();
}

void GameScene::cleanUp() {}