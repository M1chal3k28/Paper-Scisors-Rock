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
public:
    Sprite( std::shared_ptr<Texture2D> texture, int maxFrames, Vector2 frameSize );

    // Draw sprite on its position with current frame
    virtual void draw();
    // Updates sprite
    virtual void update( float deltaTime ); 

    // Set sprite texture frame
    void setFrame(int frame);

    // Checks if sprite is outside of the window
    bool isOutsideOfTheWindow();

};