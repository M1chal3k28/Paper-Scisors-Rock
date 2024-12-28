#pragma once 

#include <raylib.h>
#include <ResourceManager.hpp>

#include <Config.hpp>

// Not animated sprite
class Sprite {
protected:
    std::shared_ptr<Texture2D> sTexture;
    Vector2 sPosition;
    int sMaxFrames;
    int sFrame;
    Vector2 sFrameSize;
    Rectangle sFrameRect;
    Vector2 offset;
public:
    Sprite( std::shared_ptr<Texture2D> texture, int maxFrames, Vector2 frameSize );

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