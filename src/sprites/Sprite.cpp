#include <sprites/Sprite.hpp>

Sprite::Sprite(std::shared_ptr<Texture2D> texture, int maxFrames, Vector2 frameSize) 
    : sTexture(std::move(texture)), sPosition({0, 0}), sMaxFrames(maxFrames), sFrame(0), sFrameSize(frameSize) {
        sFrameRect = {0, 0, frameSize.x, frameSize.y};
    }

// Update sprite
void Sprite::update(float deltaTime) {}

// Draw sprite with current frame
void Sprite::draw() {
    DrawTexturePro(
        *(this->sTexture),
        (Rectangle){this->sFrame * this->sFrameSize.x, this->sFrame * this->sFrameSize.y, this->sFrameSize.x, this->sFrameSize.y}, // source rectangle sprite sheet plays role in here
        (Rectangle){this->sPosition.x, this->sPosition.y, this->sFrameSize.x, this->sFrameSize.y}, // where to display
        (Vector2){0, 0},
        0.0f,
        WHITE
    );
}

// Set sprite texture frame
void Sprite::setFrame(int frame) {
    if ( frame >= 0 && frame <= this->sMaxFrames ) {
        this->sFrame = frame;
    }
}

// Checks if sprite is outside of the screen
bool Sprite::isOutsideOfTheWindow()
{
    if ( this->sPosition.x > GetScreenWidth()  || this->sPosition.x < 0 ||
         this->sPosition.y > GetScreenHeight() || this->sPosition.y < 0 ) {
            return true;
         }
    return false;
}
