#include <sprites/RotatingSprite.hpp>

RotatingSprite::RotatingSprite(std::string textureId, int maxFrames, Vector2 frameSize, Vector2 postionOfFrameOnTexture, Vector2 velocity, float rotationSpeed)
    : MovingSprite(textureId, maxFrames, frameSize, postionOfFrameOnTexture, velocity), rotationSpeed(rotationSpeed) {}

void RotatingSprite::update(float deltaTime) {
    MovingSprite::update(deltaTime);

    this->rotation += this->rotationSpeed * deltaTime;
}

void RotatingSprite::draw() {
    DrawTexturePro(
        *RESOURCE_MANAGER.getTexture(this->sTextureId),
        (Rectangle){this->sFrame * this->sFrameSize.x, 0, this->sFrameSize.x, this->sFrameSize.y}, // source rectangle sprite sheet plays role in here
        (Rectangle){this->sPosition.x, this->sPosition.y, this->sFrameSize.x, this->sFrameSize.y}, // where to display
        this->sOffset,
        rotation,
        WHITE
    );
}
