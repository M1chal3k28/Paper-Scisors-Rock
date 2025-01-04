#include <sprites/Button.hpp>

Button::Button(Vector2 position, Vector2 sizeOfTexture, Vector2 positionOfFrameOnTexture, std::string textureId, std::function<void()> onClick) 
    : Sprite( textureId, BUTTON_MAX_STATES, sizeOfTexture, positionOfFrameOnTexture ), onClick( std::move( onClick ) ) {
        // Set position cause sprite constructor doesn't set it
        this->setPosition(position);
    }

void Button::draw() {
    // Draw button sprite with current frame
    Sprite::draw();
}

void Button::disable() {
    this->isDisabled = true;
}

void Button::enable() {
    this->isDisabled = false;
}

void Button::update(float deltaTime) {
    this->clicked = false;
    // Update button sprite
    Sprite::update(deltaTime);
    
    // Check if button is hovered
    if ( CheckCollisionPointRec(GetMousePosition(), {this->sPosition.x - this->sOffset.x, this->sPosition.y - this->sOffset.y, this->sFrameSize.x, this->sFrameSize.y}) ) {
        // If button is disabled
        if (this->isDisabled) {
            // Set button state to disabled
            SetMouseCursor(MOUSE_CURSOR_NOT_ALLOWED);
            this->sFrame = 0;
            return;
        }

        // Set mouse to cursor when hovered
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

        // Set button state to hovered
        this->sFrame = 1;

        // Check if button is clicked
        if ( IsMouseButtonDown(MOUSE_BUTTON_LEFT) ) 
            this->sFrame = 2;

        // Check if button is released
        if ( IsMouseButtonReleased(MOUSE_BUTTON_LEFT) ) {
            this->onClick();
            this->clicked = true;
        }
    } else {
        // Set button state to not hovered
        this->sFrame = 0;
    }
}

bool Button::wasClicked() {
    return this->clicked;
}