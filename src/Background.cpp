#include <Background.hpp>
#include <sprites/Sprites.hpp>

// Prepare Resources
Background::Background() {
    RESOURCE_MANAGER.loadTexture("backgroundSprite", "assets/backgroundSprite.png");

    // Create empty vector
    this->backgroundSprites = std::vector<std::unique_ptr<Sprite>>();
}

void Background::generateSprites() {
    for ( int i = 0; i < SPRITES_PER_GENERATION; ++i ) {
        // Generate random sprite
        int spriteType = RANDOM(0, SPRITE_TYPES);

        // Generate random speed
        float speedX = RANDOM(50, 300);
        float speedY = RANDOM(50, 300);

        // Generate random position X
        int posX = RANDOM(-GetScreenWidth(), GetScreenWidth());

        int posY = 0;
        
        // Generate random position Y 
        // If position is outside of the screen, generate new position Y which can be on any Y axis
        if ( posX < -(Vector2)BACKGROUND_SPRITE_SIZE.x - 10 )
            posY = RANDOM(0, GetScreenHeight());
        else
            // Generate random position Y which is above the screen
            posY = -(Vector2)BACKGROUND_SPRITE_SIZE.y - RANDOM(0, 100);

        // Generate random rotation speed
        float rotationSpeed = RANDOM(-30, 30);

        // Create sprite
        this->backgroundSprites.push_back(std::make_unique<RotatingSprite>(
            "backgroundSprite",
            2,
            (Vector2)BACKGROUND_SPRITE_SIZE,
            (Vector2){0, 0},
            (Vector2){speedX, speedY},
            rotationSpeed
        ));

        // Set sprite position
        this->backgroundSprites.back()->setPosition({(float)posX, (float)posY});
        // Set sprite frame (type of sprie) 
        this->backgroundSprites.back()->setFrame(spriteType);
    }
}

Background::~Background() {
    this->backgroundSprites.clear();
}

// Get background instance
Background &Background::getInstance() {
    static Background instance;
    return instance;
}

void Background::update(float deltaTime) {
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

void Background::draw() {
    for( auto& sprite : this->backgroundSprites ) {
        sprite->draw();
    }
}

void Background::clear() {
    this->backgroundSprites.clear();
}