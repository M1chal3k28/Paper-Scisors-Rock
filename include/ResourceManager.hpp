#pragma once

#include <Config.hpp>

#define RESOURCE_MANAGER ResourceManager::getInstance()

class ResourceManager {
public:
    // Singleton access to ResourceManager instance
    static ResourceManager& getInstance();

    // Load a texture and store it by a key
    void loadTexture(const std::string& key, const std::string& filePath);

    // Get a texture by key
    Texture2D& getTexture(const std::string& key);

    // Unload a specific texture by key
    void unloadTexture(const std::string& key);

    // Load a sound and store it by a key
    void loadSound(const std::string& key, const std::string& filePath);

    // Get a sound by key
    Sound& getSound(const std::string& key);

    // Unload a specific sound by key
    void unloadSound(const std::string& key);

    // Clean up all loaded resources
    void cleanUp();

private:
    // Private constructor for singleton pattern
    ResourceManager();
    
    // Deleted copy constructor and assignment operator
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Resource storage
    std::unordered_map<std::string, Texture2D> textures;
    std::unordered_map<std::string, Sound> sounds;
};
