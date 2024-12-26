#pragma once

#include <ResourceManager.hpp>
#include <sprites/Sprites.hpp>

#define M_BG Background::getInstance()

class Background {
    Background();
    ~Background();

    Background(const Background&) = delete;
    Background& operator=(const Background&) = delete;

    std::vector<std::unique_ptr<Sprite>> backgroundSprites;

public:
    static Background& getInstance();
    
    // Update position of sprites
    void update( float deltaTime );

    // Draw background
    void draw();

};