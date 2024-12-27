#include <Button.hpp>

Button::Button(Vector2 position, Vector2 sizeOfButton, std::string text, std::shared_ptr<Texture2D> texture, std::shared_ptr<Font> font, std::function<void()> onClick) 
 : Sprite( texture, BUTTON_MAX_STATES, sizeOfButton ), text(text), font(font) {
    // Move function to the object variable
    this->onClick = std::move( onClick );

    // Set offset to the center of the object
    this->offset = Vector2{sizeOfButton.x / 2, sizeOfButton.y / 2};

    // Set position cause sprite constructor doesn't set it
    this->setPosition(position);

    // Set text offset and position
    Vector2 textSize = MeasureTextEx(*(this->font), this->text.c_str(), TEXT_SIZE, TEXT_SPACING);
    this->textOffset = Vector2{ textSize.x / 2, textSize.y / 2 };
    this->textPosition = Vector2{ this->sPosition.x, this->sPosition.y };
}

void Button::draw() {
    // Draw button sprite with current frame
    DrawTexturePro(
        *(this->sTexture), 
        {this->sFrameSize.x * this->sFrame, 0, this->sFrameSize.x , this->sFrameSize.y}, 
        {this->sPosition.x, this->sPosition.y, this->sFrameSize.x, this->sFrameSize.y}, 
        this->offset, 
        0, 
        WHITE
    );

    // Draw button text
    // in the center of the button
    DrawTextPro(
        *(this->font),
        this->text.c_str(),
        this->textPosition,
        this->textOffset,
        0,
        TEXT_SIZE,
        TEXT_SPACING,
        WHITE
    );
}

void Button::update() {
    // Update button sprite

    // Check if button is hovered
    if ( CheckCollisionPointRec(GetMousePosition(), {this->sPosition.x - this->offset.x, this->sPosition.y - this->offset.y, this->sFrameSize.x, this->sFrameSize.y}) ) {
        // Set mouse to cursor when hovered
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

        // Set button state to hovered
        this->sFrame = 1;

        // Check if button is clicked
        if ( IsMouseButtonDown(MOUSE_BUTTON_LEFT) ) 
            this->sFrame = 2;

        // Check if button is released
        if ( IsMouseButtonReleased(MOUSE_BUTTON_LEFT) )
            this->onClick();
    } else {
        // Set button state to not hovered
        this->sFrame = 0;
    }
}