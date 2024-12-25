#pragma once

#include <Config.hpp>

class Scene {
public:
    // Function to prepare resources needed by the scene
    virtual void prepareResources() = 0;

    // Function to update the scene's state (e.g., handle input, animations, etc.)
    virtual void update(float deltaTime) = 0;

    // Function to render the scene
    virtual void draw() const = 0;

    // Function to clean up resources when the scene is no longer needed
    virtual void cleanUp() = 0;
};