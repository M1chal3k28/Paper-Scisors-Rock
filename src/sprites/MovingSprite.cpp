#include <sprites/MovingSprite.hpp>
#include <raymath.h>

MovingSprite::MovingSprite(std::string textureId, int maxFrames, Vector2 frameSize, Vector2 velocity)
    : Sprite(textureId, maxFrames, frameSize), msVelocity(velocity) {}

void MovingSprite::update(float deltaTime) {
    // Update position based on velocity
    this->sPosition += this->msVelocity * deltaTime;
}

void MovingSprite::draw() {
    Sprite::draw();
}