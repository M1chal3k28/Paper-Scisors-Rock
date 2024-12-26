#include <Background.hpp>
#include <sprites/Sprites.hpp>

// Prepare Resources
Background::Background() {
    RESOURCE_MANAGER.loadTexture("backgroundSprite", "assets/backgroundSprite.png");

    // Create empty vector
    this->backgroundSprites = std::vector<std::unique_ptr<Sprite>>();

    // Add test sprite
    this->backgroundSprites.push_back(std::make_unique<MovingSprite>(MovingSprite(
        RESOURCE_MANAGER.getTexture("backgroundSprite"),
        2,
        {80.f, 100.f},
        {100.f, 70.f}
    )));
    this->backgroundSprites.push_back(std::make_unique<MovingSprite>(MovingSprite(
        RESOURCE_MANAGER.getTexture("backgroundSprite"),
        2,
        {80.f, 100.f},
        {200.f, 150.f}
    )));
}

// Unload Resources
Background::~Background() {
    RESOURCE_MANAGER.unloadTexture("backgroundSprite");
}

// Get background instance
Background &Background::getInstance() {
    static Background instance;
    return instance;
}

void Background::update(float deltaTime)
{
    for( auto& sprite : this->backgroundSprites ) {
        sprite->update(deltaTime);
    }
}

void Background::draw()
{
    for( auto& sprite : this->backgroundSprites ) {
        sprite->draw();
    }
}