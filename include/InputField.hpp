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
    std::shared_ptr<Font> font;
    Vector2 scale;
public:
    InputField(Vector2 position, Vector2 sizeOfButton /* In the sheet (texture) */, std::shared_ptr<Texture2D> texture, std::shared_ptr<Font> font, Vector2 scale);
    virtual void draw();
    virtual void update(float deltaTime) override; 
};