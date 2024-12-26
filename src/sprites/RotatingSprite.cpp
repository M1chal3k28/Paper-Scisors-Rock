#include <sprites/RotatingSprite.hpp>

RotatingSprite::RotatingSprite(std::shared_ptr<Texture2D> texture, int maxFrames, Vector2 frameSize, Vector2 velocity, float rotationSpeed)
    : MovingSprite(texture, maxFrames, frameSize, velocity), rotationSpeed(rotationSpeed) {}

void RotatingSprite::update(float deltaTime) {
    MovingSprite::update(deltaTime);

    this->rotation += this->rotationSpeed * deltaTime;
}

void RotatingSprite::draw() {
    DrawTexturePro(
        *(this->sTexture),
        (Rectangle){this->sFrame * this->sFrameSize.x, 0, this->sFrameSize.x, this->sFrameSize.y}, // source rectangle sprite sheet plays role in here
        (Rectangle){this->sPosition.x, this->sPosition.y, this->sFrameSize.x, this->sFrameSize.y}, // where to display
        this->offset,
        rotation,
        WHITE
    );
}
