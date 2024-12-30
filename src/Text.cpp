#include <Text.hpp>

#include <ResourceManager.hpp>

Text::Text(std::string text, Vector2 offset, Vector2 position, std::string fontId, float size, float spacing, Color color = WHITE)
    : tText(text), tOffset(offset), tPosition(position), tFontId(fontId), tSize(size), tSpacing(spacing), tColor(color) {}

void Text::draw() {
    DrawTextPro(
        *RESOURCE_MANAGER.getFont(this->tFontId),
        this->tText.c_str(),
        this->tPosition,
        this->tOffset,
        0,
        this->tSize,
        this->tSpacing,
        this->tColor
    );
}

void Text::tSetPosition(Vector2 position) {
    this->tPosition = position;
}

Vector2 Text::tMeasure()
{
    this->tMeasurement = MeasureTextEx(*RESOURCE_MANAGER.getFont(this->tFontId), this->tText.c_str(), this->tSize, this->tSpacing);
    return this->tMeasurement;
}

void Text::tCenterOffset() {
    this->tMeasure();

    this->tOffset = {this->tMeasurement.x / 2, this->tMeasurement.y / 2};
}
