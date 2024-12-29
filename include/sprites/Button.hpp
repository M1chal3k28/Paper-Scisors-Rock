#pragma once

#include <Config.hpp>

#include <sprites/Sprite.hpp>

#define BUTTON_MAX_STATES 2 // 0 is not hovered, 1 is hovered 2 is pressed
#define MENU_BUTTON_SIZE {600.f, 100.f}
#define TEXT_SIZE 40.f

class Button : public Sprite {
    std::string text;
    std::string fontId;
    std::function<void()> onClick;
    Vector2 textOffset;
    Vector2 textPosition;
public:
    Button(
        Vector2 position, // position of the button
        Vector2 sizeOfTexture, // size of the button in the sheet
        Vector2 positionOfFrameOnTexture, // Top left of the frame in the sheet
        std::string text, // Text to display on the button
        std::string textureId, // Texture to use
        std::string fontId, // Font to use
        std::function<void()> onClick // Function to call when button is clicked
    );
    
    // Updates button states
    // Checks if button is hovered, clicked
    void update();

    // Draws the button in the given position
    virtual void draw() override;
};