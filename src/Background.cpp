#include <Background.hpp>
#include <sprites/Sprites.hpp>

// Prepare Resources
Background::Background() {
    RESOURCE_MANAGER.loadTexture("backgroundSprite", "assets/backgroundSprite.png");

    // Create empty vector
    this->backgroundSprites = std::vector<std::unique_ptr<Sprite>>();

    // Add test sprite
    this->backgroundSprites.emplace_back(new MovingSprite(
        RESOURCE_MANAGER.getTexture("backgroundSprite"),
        2,
        {80.f, 100.f},
        {100.f, 70.f}
    ));
    this->backgroundSprites.emplace_back(new MovingSprite(
        RESOURCE_MANAGER.getTexture("backgroundSprite"),
        2,
        {80.f, 100.f},
        {200.f, 150.f}
    ));
}

// Unload Resources
Background::~Background() {
    RESOURCE_MANAGER.unloadTexture("backgroundSprite");
}

void Background::generateSprites() {
    for ( int i = 0; i < SPRITES_PER_GENERATION; ++i ) {
        int spriteType = RANDOM(0, SPRITE_TYPES);

        float speedX = RANDOM(50, 300);
        float speedY = RANDOM(50, 300);

        int posX = RANDOM(-GetScreenWidth(), GetScreenWidth());
        int posY = -(Vector2)BACKGROUND_SPRITE_SIZE.y - RANDOM(0, 100);

        this->backgroundSprites.emplace_back(new MovingSprite(
            RESOURCE_MANAGER.getTexture("backgroundSprite"),
            2,
            BACKGROUND_SPRITE_SIZE,
            {speedX, speedY}
        ));

        this->backgroundSprites.back()->setPosition({(float)posX, (float)posY});
        this->backgroundSprites.back()->setFrame(spriteType);
    }
}

// Get background instance
Background &Background::getInstance() {
    static Background instance;
    return instance;
}

void Background::update(float deltaTime)
{
    // While updating check if any is outside and flag its id to deletion
    for ( size_t i = 0; i < backgroundSprites.size(); ++i ) {
        // Get sprite for simplicity
        auto* sprite = backgroundSprites[i].get();

        // Update sprite
        sprite->update(deltaTime);

        // Check if sprite is outside right and bottom of the screen
        if ( sprite->isOutsideOfTheWindowSide(ScreenSide::RIGHT) || sprite->isOutsideOfTheWindowSide(ScreenSide::BOTTOM) ) {
            this->backgroundSprites.erase(this->backgroundSprites.begin() + i);
            i--;
        }
    }

    // Check timer
    this->generationTimer += deltaTime;
    if ( this->generationTimer >= GENERATION_DELAY ) {
        // Generate new sprites for background
        this->generateSprites();
        this->generationTimer = 0.f;
    }
}

void Background::draw()
{
    for( auto& sprite : this->backgroundSprites ) {
        sprite->draw();
    }
}