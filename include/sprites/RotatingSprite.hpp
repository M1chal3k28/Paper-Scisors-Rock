#pragma once

#include <sprites/Sprites.hpp>

class RotatingSprite : public MovingSprite {
    float rotationSpeed;
public:
    RotatingSprite(std::string textureId, int maxFrames, Vector2 frameSize, Vector2 postionOfFrameOnTexture, Vector2 velocity, float rotationSpeed);

    // Update rotation and position
    virtual void update(float deltaTime);

    // Draw the sprite with rotation
    virtual void draw() override;
};