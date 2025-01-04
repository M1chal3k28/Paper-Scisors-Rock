#include <sprites/RotatingSprite.hpp>

RotatingSprite::RotatingSprite(std::string textureId, int maxFrames, Vector2 frameSize, Vector2 postionOfFrameOnTexture, Vector2 velocity, float rotationSpeed)
    : MovingSprite(textureId, maxFrames, frameSize, postionOfFrameOnTexture, velocity), rotationSpeed(rotationSpeed) {}

void RotatingSprite::update(float deltaTime) {
    MovingSprite::update(deltaTime);

    this->sRotation += this->rotationSpeed * deltaTime;
}

void RotatingSprite::draw() {
    Sprite::draw();
}
