#pragma once 

#include <raylib.h>
#include <ResourceManager.hpp>

#include <Config.hpp>

// Not animated sprite
class Sprite {
protected:
    std::string sTextureId;
    Vector2 sPosition;
    int sMaxFrames;
    int sFrame;
    Vector2 sFrameSize;
    Rectangle sFrameRect;
    Vector2 sOffset;
    Vector2 sPositionOfFrameOnTexture;
public:
    Sprite( std::string textureId, int maxFrames, Vector2 frameSize, Vector2 positionOfFrameOnTexture );
    ~Sprite() = default;

    // Draw sprite on its position with current frame
    virtual void draw();
    
    // Draw sprite on its position with current frame and give size
    void draw(Vector2 size);
    // Updates sprite
    virtual void update( float deltaTime ); 

    // Set sprite texture frame
    void setFrame(int frame);
    void setPosition(Vector2 pos);

    // Checks if sprite is outside of the window
    bool isOutsideOfTheWindow();

    // Checks if sprite is outside of the window side
    bool isOutsideOfTheWindowSide( ScreenSide::Value side );
};