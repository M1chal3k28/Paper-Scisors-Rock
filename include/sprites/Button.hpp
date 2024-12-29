#pragma once

#include <Config.hpp>

#include <sprites/Sprite.hpp>

class Button : public Sprite {
protected:
    std::function<void()> onClick;
    Vector2 textPosition;
public:
    Button(
        Vector2 position, // position of the button
        Vector2 sizeOfTexture, // size of the button in the sheet
        Vector2 positionOfFrameOnTexture, // Top left of the frame in the sheet
        std::string textureId, // Texture to use
        std::function<void()> onClick // Function to call when button is clicked
    );
    
    // Updates button states
    // Checks if button is hovered, clicked
    virtual void update(float deltaTime);

    // Draws the button in the given position
    virtual void draw();
};