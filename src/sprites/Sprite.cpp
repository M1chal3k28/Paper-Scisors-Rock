#include <sprites/Sprite.hpp>

Sprite::Sprite(std::shared_ptr<Texture2D> texture, int maxFrames, Vector2 frameSize) 
    : sTexture( texture ), sPosition({0, 0}), sMaxFrames(maxFrames), sFrame(0), sFrameSize(frameSize) {
        sFrameRect = {0, 0, frameSize.x, frameSize.y};
    }

Sprite::~Sprite() {
    std::cout << "Deleted Sprite\n";
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

void Sprite::setPosition(Vector2 pos) {
    this->sPosition = pos;
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

// Checks if sprite is outside of the window side
bool Sprite::isOutsideOfTheWindowSide(ScreenSide::Value side) {
    switch (side) {
        case ScreenSide::Value::LEFT:
            return this->sPosition.x < 0;
        case ScreenSide::Value::RIGHT:
            return this->sPosition.x > GetScreenWidth();
        case ScreenSide::Value::TOP:
            return this->sPosition.y < 0;
        case ScreenSide::Value::BOTTOM:
            return this->sPosition.y > GetScreenHeight();
    }
    return false;
}
