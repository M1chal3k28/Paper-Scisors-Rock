#include <sprites/Sprite.hpp>

#include <ResourceManager.hpp>

// Constructor
Sprite::Sprite(std::string textureId, int maxFrames, Vector2 frameSize, Vector2 positionOfFrameOnTexture = Vector2{0, 0} /* Left top corner */) 
    : sTextureId( textureId ), sPosition({0, 0}), sMaxFrames(maxFrames), sFrame(0), sFrameSize(frameSize), sPositionOfFrameOnTexture(positionOfFrameOnTexture) {
        // Set offset to the center
        sOffset = {this->sFrameSize.x / 2, this->sFrameSize.y / 2};
    }


// Update sprite
void Sprite::update(float deltaTime) {}

// Draw sprite with current frame
void Sprite::draw() {
    DrawTexturePro(
        *RESOURCE_MANAGER.getTexture(this->sTextureId),
        (Rectangle){
            this->sPositionOfFrameOnTexture.x + ( this->sFrame * this->sFrameSize.x ), // X offset on texture 
            this->sPositionOfFrameOnTexture.y, // Y offset on texture
            this->sFrameSize.x, // Size of the frame X
            this->sFrameSize.y // Size of the frame Y
        }, // source rectangle sprite sheet plays role in here
        (Rectangle){this->sPosition.x, this->sPosition.y, this->sFrameSize.x, this->sFrameSize.y}, // where to display
        this->sOffset,
        0.0f,
        WHITE
    );
}

// Draw sprite with current frame and give sizeing
void Sprite::draw(Vector2 size) {
    DrawTexturePro(
        *RESOURCE_MANAGER.getTexture(this->sTextureId),
        (Rectangle){
            this->sPositionOfFrameOnTexture.x + ( this->sFrame * this->sFrameSize.x ), // X offset on texture 
            this->sPositionOfFrameOnTexture.y, // Y offset on texture
            this->sFrameSize.x, // Size of the frame X
            this->sFrameSize.y // Size of the frame Y
        }, // source rectangle sprite sheet plays role in here
        (Rectangle){this->sPosition.x, this->sPosition.y, size.x, size.y}, // where to display
        this->sOffset,
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

// Set sprite position
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
