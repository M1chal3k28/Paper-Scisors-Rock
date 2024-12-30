#pragma once

#include <sprites/Button.hpp>
#include <Text.hpp>

class TextButton : public Button, private Text {
public:
    TextButton(
        Vector2 position, // position of the button
        Vector2 sizeOfTexture, // size of the button texture
        Vector2 positionOfFrameOnTexture, // Top left of the frame in the sheet
        std::string text, // Text to display
        std::string textureId, // Texture to use
        std::string fontId, // Font to use
        std::function<void()> onClick // Function to call when button is clicked
    );

    virtual void draw();
    virtual void update(float deltaTime);
};