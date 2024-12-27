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
    Color color = {255, 255, 255, (char)255 * (timer / INTRO_SCENE_DURATION)};
    // DrawTexturePro(*RESOURCE_MANAGER.getTexture("intro"), {0, 0, 2560, 1440}, {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, {0, 0}, 0, color);

    // Texture size
    float textureWidth = 2560;
    float textureHeight = 1440;

    // Screen size
    float screenWidth = (float)GetScreenWidth();
    float screenHeight = (float)GetScreenHeight();

    // Aspect ratio
    float aspectTexture = textureWidth / textureHeight;
    float aspectScreen = screenWidth / screenHeight;

    Rectangle destRec;

    if (aspectScreen > aspectTexture) {
        // Scale within the screen height
        destRec.height = screenHeight;
        destRec.width = screenHeight * aspectTexture;
        destRec.x = (screenWidth - destRec.width) / 2;  // center horizontally
        destRec.y = 0;
    } else {
        // Scale within the screen width
        destRec.width = screenWidth;
        destRec.height = screenWidth / aspectTexture;
        destRec.x = 0;
        destRec.y = (screenHeight - destRec.height) / 2;  // center vertically
    }

    DrawTexturePro(
        *RESOURCE_MANAGER.getTexture("intro"),
        {0, 0, textureWidth, textureHeight}, 
        destRec,                             
        {0, 0},                              
        0,                                   
        WHITE                                
    );
}

void IntroScene::cleanUp() {
    RESOURCE_MANAGER.unloadTexture("intro");
}