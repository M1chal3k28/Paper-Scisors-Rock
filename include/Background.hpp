#pragma once

#include <ResourceManager.hpp>
#include <sprites/Sprites.hpp>

#define BACKGROUND_SPRITE_SIZE {80.f, 100.f}
#define SPRITE_TYPES 2 // 0 is rock, 1 is scissors, 2 is paper

#define M_BG Background::getInstance()
#define GENERATION_DELAY 0.5f
#define SPRITES_PER_GENERATION 10
class Background {
    Background();
    ~Background();

    Background(const Background&) = delete;
    Background& operator=(const Background&) = delete;

    std::vector<std::unique_ptr<Sprite>> backgroundSprites;

    float generationTimer = 0.f;

    // Generate new sprites for background
    void generateSprites();

public:
    // Get background instance
    static Background& getInstance();
    
    // Update position of sprites
    void update( float deltaTime );

    // Draw background
    void draw();
};