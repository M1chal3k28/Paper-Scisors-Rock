#pragma once 

#include <Config.hpp>

class Text {
protected:
    std::string tText;
    float tSize;
    float tSpacing;
    Vector2 tOffset;
    Vector2 tPosition;
    std::string tFontId;
    Vector2 tMeasurement;
public:
    Text(std::string text, Vector2 offset, Vector2 position, std::string fontId, float size, float spacing);
    virtual void draw();

    void tSetPosition( Vector2 position );
    Vector2 tMeasure();
    void tCenterOffset();
};