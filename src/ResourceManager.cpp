#include <ResourceManager.hpp>

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

ResourceManager::ResourceManager() {}

void ResourceManager::loadTexture(const std::string& key, const std::string& filePath) {
    if (textures.find(key) == textures.end()) {
        textures[key] = LoadTexture(filePath.c_str());
    }
}

Texture2D& ResourceManager::getTexture(const std::string& key) {
    return textures.at(key);
}

void ResourceManager::unloadTexture(const std::string& key) {
    if (textures.find(key) != textures.end()) {
        UnloadTexture(textures[key]);
        textures.erase(key);
    }
}

void ResourceManager::loadSound(const std::string& key, const std::string& filePath) {
    if (sounds.find(key) == sounds.end()) {
        sounds[key] = LoadSound(filePath.c_str());
    }
}

Sound& ResourceManager::getSound(const std::string& key) {
    return sounds.at(key);
}

void ResourceManager::unloadSound(const std::string& key) {
    if (sounds.find(key) != sounds.end()) {
        UnloadSound(sounds[key]);
        sounds.erase(key);
    }
}

void ResourceManager::cleanUp() {
    for (auto& [key, texture] : textures) {
        UnloadTexture(texture);
    }
    textures.clear();

    for (auto& [key, sound] : sounds) {
        UnloadSound(sound);
    }
    sounds.clear();
}
