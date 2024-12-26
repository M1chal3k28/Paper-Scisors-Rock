#pragma once

#include <sprites/MovingSprite.hpp>
#include <raymath.h>

MovingSprite::MovingSprite(std::shared_ptr<Texture2D> texture, int maxFrames, Vector2 frameSize, Vector2 velocity)
    : Sprite(texture, maxFrames, frameSize), msVelocity(velocity) {}

void MovingSprite::update(float deltaTime) {
    // Update position based on velocity
    this->sPosition += this->msVelocity * deltaTime;
}

void MovingSprite::draw() {
    Sprite::draw();
}