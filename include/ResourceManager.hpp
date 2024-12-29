#pragma once

#include <Config.hpp>

#define RESOURCE_MANAGER ResourceManager::getInstance()

class ResourceManager {
public:
    // Singleton access to ResourceManager instance
    static ResourceManager& getInstance();

    // Clear all loaded resources
    ~ResourceManager();

    // Load a texture and store it by a key
    void loadTexture(const std::string& key, const std::string& filePath);

    // Get a texture by key
    std::shared_ptr<Texture2D> getTexture(const std::string& key);

    // Unload a specific texture by key
    void unloadTexture(const std::string& key);

    // Load a sound and store it by a key
    void loadSound(const std::string& key, const std::string& filePath);

    // Get a sound by key
    std::shared_ptr<Sound> getSound(const std::string& key);

    // Unload a specific sound by key
    void unloadSound(const std::string& key);

    // Load a font and store it by a key
    void loadFont(const std::string& key, const std::string& filePath);

    // Get a font by key
    std::shared_ptr<Font> getFont(const std::string& key);

    // Unload a specific font by key
    void unloadFont(const std::string& key);

    // Clean up all loaded resources
    void cleanUp();

private:
    // Private constructor for singleton pattern
    ResourceManager();
    
    // Deleted copy constructor and assignment operator
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Resource storage
    std::unordered_map<std::string, std::shared_ptr<Texture2D>> textures;
    std::unordered_map<std::string, std::shared_ptr<Sound>> sounds;
    std::unordered_map<std::string, std::shared_ptr<Font>> fonts;
};
