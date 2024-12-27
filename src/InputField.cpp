#include <InputField.hpp>

InputField::InputField(Vector2 position, Vector2 sizeOfButton /* In the sheet (texture) */, std::shared_ptr<Texture2D> texture, std::shared_ptr<Font> font, Vector2 scale) 
    : Sprite(texture, 1, sizeOfButton), scale(scale) {
    // Set font
    this->font = font;
    // Set offset to the center
    this->offset = {scale.x / 2, scale.y / 2};

    // Set text size 
    this->textSize = this->scale.y - 10;

    // Set sprite position
    this->setPosition(position);
}

void InputField::draw() {
    // Draw texture
    Sprite::draw(this->scale);

    // Draw text
    DrawTextPro(
        *(this->font),
        this->text,
        this->sPosition,
        this->textOffset,
        0,
        this->textSize,
        TEXT_SPACING,
        WHITE
    );
}

void InputField::update(float deltaTime) {
    // Check if mouse is on the input field
    if ( CheckCollisionPointRec(GetMousePosition(), {this->sPosition.x - this->offset.x, this->sPosition.y - this->offset.y, this->scale.x, this->scale.y}) ) {
        this->isFocused = true;
        this->sFrame = 1;
    } else { 
        this->sFrame = 0;
        this->isFocused = false;
    }
    // Check if any character has been pressed
    bool anyChange = false;

    if ( this->isFocused ) {
        // Set the window's cursor to the I-Beam
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) && (this->cursorPos < TEXT_LENGTH))
            {
                this->text[this->cursorPos] = (char)key;
                this->text[this->cursorPos+1] = '\0'; // Add null terminator at the end of the string.
                this->cursorPos++;
            }

            key = GetCharPressed();  // Check next character in the queue

            anyChange = true; // At least one key was pressed
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            this->cursorPos--;
            if (this->cursorPos < 0) this->cursorPos = 0;
            this->text[this->cursorPos] = '\0';

            anyChange = true; // At least one key was pressed
        }

        // If any change was made, update the text offset
        if(anyChange) {
            // Update text offset
            this->textOffset = MeasureTextEx(*(this->font), this->text, this->textSize, TEXT_SPACING);
            this->textOffset = {this->textOffset.x / 2, this->textOffset.y / 2};
        }
    }
}
