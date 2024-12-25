#include <IntroScene.hpp>

#include <SceneManager.hpp>

IntroScene::~IntroScene() {
    cleanUp();
}

IntroScene::IntroScene() {
    prepareResources();
}

void IntroScene::prepareResources() {
    RESOURCE_MANAGER.loadTexture("intro", "assets/intro.png");
}

void IntroScene::update(float deltaTime) {
    timer += deltaTime;
    if (timer > INTRO_SCENE_DURATION) {
        SCENE_MANAGER.pushScene( SceneType::INTRO );
    }
}

void IntroScene::draw() const {
    Color color = {255, 255, 255, 255 * (timer / INTRO_SCENE_DURATION)};
    DrawTexturePro(RESOURCE_MANAGER.getTexture("intro"), {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, {0, 0}, 0, color);
}

void IntroScene::cleanUp() {
    RESOURCE_MANAGER.unloadTexture("intro");
}