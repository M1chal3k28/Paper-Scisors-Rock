#pragma once

#include <sprites/Sprite.hpp>

class MovingSprite : public Sprite {
    Vector2 msVelocity;
public:
    MovingSprite( std::shared_ptr<Texture2D> texture, int maxFrames, Vector2 frameSize, Vector2 velocity );

    virtual void update( float deltaTime );
    virtual void draw();
};