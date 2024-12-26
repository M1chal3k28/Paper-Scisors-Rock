#pragma once

#include <sprites/Sprites.hpp>

class RotatingSprite : public MovingSprite {
    float rotation;
    float rotationSpeed;
public:
    RotatingSprite(std::shared_ptr<Texture2D> texture, int maxFrames, Vector2 frameSize, Vector2 velocity, float rotationSpeed);

    // Update rotation and position
    virtual void update(float deltaTime);

    // Draw the sprite with rotation
    virtual void draw() override;
};