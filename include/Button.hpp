#pragma once

#include <Config.hpp>

#include <sprites/Sprite.hpp>

#define BUTTON_MAX_STATES 2 // 0 is not hovered, 1 is hovered 2 is pressed
#define MENU_BUTTON_SIZE {600.f, 100.f}
#define TEXT_SIZE 40.f

class Button : public Sprite {
    std::string text;
    std::shared_ptr<Font> font;
    std::function<void()> onClick;
    Vector2 textOffset;
    Vector2 textPosition;
public:
    Button(Vector2 position, Vector2 sizeOfButton /* In the sheet (texture) */, std::string text, std::string textureId, std::shared_ptr<Font> font, std::function<void()> onClick);
    
    // Updates button states
    // Checks if button is hovered, clicked
    void update();

    // Draws the button in the given position
    virtual void draw() override;
};