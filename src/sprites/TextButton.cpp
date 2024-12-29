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
), text(text), fontId(fontId) {
    // Set text offset and position
    Vector2 textSize = MeasureTextEx(*RESOURCE_MANAGER.getFont(this->fontId), this->text.c_str(), TEXT_SIZE, TEXT_SPACING);
    this->textOffset = Vector2{ textSize.x / 2.f, textSize.y / 2.f };
    this->textPosition = Vector2{ this->sPosition.x, this->sPosition.y };
}

void TextButton::draw() {
    // Draw button sprite with current frame
    Button::draw();

    // Draw button text
    // in the center of the button
    DrawTextPro(
        *RESOURCE_MANAGER.getFont(this->fontId),
        this->text.c_str(),
        this->textPosition,
        this->textOffset,
        0,
        TEXT_SIZE,
        TEXT_SPACING,
        WHITE
    );
}

void TextButton::update(float deltaTime) {
    // Update button sprite
    Button::update(deltaTime);
}