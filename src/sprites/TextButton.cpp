#include <sprites/TextButton.hpp>

// Create a new text button
// Center the text
TextButton::TextButton(
    Vector2 position, 
    Vector2 sizeOfTexture, 
    Vector2 positionOfFrameOnTexture, 
    std::string text, 
    std::string textureId, 
    std::string fontId, 
    std::function<void()> onClick
) : Button(
    position, 
    sizeOfTexture, 
    positionOfFrameOnTexture, 
    textureId, 
    onClick
), Text(text, Vector2{0, 0}, Vector2{0, 0}, fontId, TEXT_SIZE, TEXT_SPACING, WHITE) {
    // Set text offset and position
    this->tCenterOffset();
    this->tSetPosition(this->sPosition);
}

void TextButton::draw() {
    // Draw button sprite with current frame
    Button::draw();

    // Draw button text
    Text::draw();
}

void TextButton::update(float deltaTime) {
    // Update button sprite
    Button::update(deltaTime);
}