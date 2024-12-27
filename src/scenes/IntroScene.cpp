#include <scenes/IntroScene.hpp>

#include <scenes/SceneManager.hpp>
#include <scenes/SetupScene.hpp>

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
        SCENE_MANAGER.pushScene( SceneType::SETUP );
    }
}

void IntroScene::draw() const {
    // Set transparency of the intro image
    unsigned char alpha = 255 * (timer / INTRO_SCENE_DURATION);
    Color color = {255, 255, 255, alpha};

    // Texture size
    static float textureWidth = 2560;
    static float textureHeight = 1440;

    DrawTexturePro(
        *RESOURCE_MANAGER.getTexture("intro"),
        {0, 0, textureWidth, textureHeight}, 
        {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},                             
        {0, 0},                              
        0,                                   
        color                                
    );
}

void IntroScene::cleanUp() {
    RESOURCE_MANAGER.unloadTexture("intro");
}