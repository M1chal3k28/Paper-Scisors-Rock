#pragma once

#include <Config.hpp>

#define TEXT_LENGTH 10

class InputField {
    Rectangle rectangle;
    char text[TEXT_LENGTH + 1] = "\0"; 
    int cursorPos = 0;
    bool isFocused = false;
public:
    InputField(Rectangle rect);
    void draw() const;
    void update();
};