#pragma once 

#include <string>
#include <unordered_map>
#include <memory>
#include <raylib.h>

class ResourceManager {
public:
    // Singleton access to ResourceManager instance
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    // Load a PNG file and store it by a key
    void loadPNG(const std::string& key, const std::string& filePath) {
        if (pngFiles.find(key) == pngFiles.end()) {
            Image image = LoadImage(filePath.c_str());
            pngFiles[key] = image;
        }
    }

    // Get a PNG file by key
    Image& getPNG(const std::string& key) {
        return pngFiles.at(key);
    }

    // Unload a specific PNG file by key
    void unloadPNG(const std::string& key) {
        if (pngFiles.find(key) != pngFiles.end()) {
            UnloadImage(pngFiles[key]);
            pngFiles.erase(key);
        }
    }

    // Load a sound and store it by a key
    void loadSound(const std::string& key, const std::string& filePath) {
        if (sounds.find(key) == sounds.end()) {
            sounds[key] = LoadSound(filePath.c_str());
        }
    }

    // Get a sound by key
    Sound& getSound(const std::string& key) {
        return sounds.at(key);
    }

    // Unload a specific sound by key
    void unloadSound(const std::string& key) {
        if (sounds.find(key) != sounds.end()) {
            UnloadSound(sounds[key]);
            sounds.erase(key);
        }
    }

    // Clean up all loaded resources
    void cleanUp() {
        for (auto& [key, image] : pngFiles) {
            UnloadImage(image);
        }
        pngFiles.clear();

        for (auto& [key, sound] : sounds) {
            UnloadSound(sound);
        }
        sounds.clear();
    }

private:
    // Private constructor for singleton pattern
    ResourceManager() {}
    
    // Deleted copy constructor and assignment operator
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Resource storage
    std::unordered_map<std::string, Image> pngFiles;
    std::unordered_map<std::string, Sound> sounds;
};