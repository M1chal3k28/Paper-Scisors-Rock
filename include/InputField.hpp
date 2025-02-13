#pragma once

#include <Config.hpp>

#include <sprites/Sprite.hpp>

#define TEXT_LENGTH 10

class InputField : public Sprite {
    Rectangle rectangle;
    Vector2 textOffset;
    char text[TEXT_LENGTH + 1] = "\0"; 
    int cursorPos = 0;
    bool isFocused = false;
    float textSize;
    std::string fontId;
    Vector2 actualSize;
public:
    InputField(
        Vector2 position, 
        Vector2 sizeOfTexture /* In the sheet (texture) */, 
        Vector2 positionOfFrameOnTexture,
        std::string textureId, 
        std::string fontId, 
        Vector2 scaleToSize
    );
    InputField(
        Vector2 position, 
        Vector2 sizeOfTexture /* In the sheet (texture) */, 
        Vector2 positionOfFrameOnTexture,
        std::string textureId, 
        std::string fontId
    );
    virtual void draw();
    virtual void update(float deltaTime) override; 

    std::string getText();
    void clear();
};