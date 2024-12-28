#include <InputField.hpp>

InputField::InputField(Vector2 position, Vector2 sizeOfTexture /* In the sheet (texture) */, std::string textureId, std::string fontId, Vector2 actualSize) 
    : Sprite(textureId, 1, sizeOfTexture), actualSize(actualSize), fontId(fontId) {
    // Set offset to the center
    this->offset = {actualSize.x / 2, actualSize.y / 2};
    // Set text size 
    this->textSize = this->actualSize.y - 30;

    // Set sprite position
    this->setPosition(position);
}

InputField::InputField(Vector2 position, Vector2 sizeOfTexture /* In the sheet (texture) */, std::string textureId, std::string fontId) 
    : Sprite(textureId, 1, sizeOfTexture), actualSize(sizeOfTexture), fontId(fontId) {
    // Set offset to the center
    this->offset = {this->actualSize.x / 2, this->actualSize.y / 2};
    // Set text size 
    this->textSize = this->actualSize.y - 30;

    // Set sprite position
    this->setPosition(position);
}

void InputField::draw() {
    // Draw texture
    Sprite::draw(this->actualSize);

    // Draw text
    DrawTextPro(
        *RESOURCE_MANAGER.getFont(this->fontId),
        this->text,
        this->sPosition,
        this->textOffset,
        0,
        this->textSize,
        TEXT_SPACING,
        WHITE
    );

    Vector2 textMeausure = MeasureTextEx(*RESOURCE_MANAGER.getFont(this->fontId), this->text, this->textSize, TEXT_SPACING);
    
    // Draw underscore
    if ( this->isFocused ) {
        if ( this->cursorPos > 0 ) 
            DrawTextPro(
                *RESOURCE_MANAGER.getFont(this->fontId),
                "_",
                {this->sPosition.x + textMeausure.x / 2.f, this->sPosition.y - textMeausure.y / 2.f},
                {0, 0},
                0,
                this->textSize,
                TEXT_SPACING,
                WHITE
            );
        else 
            DrawTextPro(
                *RESOURCE_MANAGER.getFont(this->fontId),
                "_",
                {this->sPosition.x, this->sPosition.y - this->textSize / 2.f},
                {0, 0},
                0,
                this->textSize,
                TEXT_SPACING,
                WHITE
            );
    }
}

void InputField::update(float deltaTime) {
    // Check if mouse is on the input field
    if ( CheckCollisionPointRec(GetMousePosition(), {this->sPosition.x - this->offset.x, this->sPosition.y - this->offset.y, this->actualSize.x, this->actualSize.y}) ) {
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
            this->textOffset = MeasureTextEx(*RESOURCE_MANAGER.getFont(this->fontId), this->text, this->textSize, TEXT_SPACING);
            this->textOffset = {this->textOffset.x / 2.f, this->textOffset.y / 2.f};
        }
    }
}

std::string InputField::getText() {
    return std::string(this->text);
}

void InputField::clear() {
    this->text[0] = '\0';
    this->cursorPos = 0;
}
