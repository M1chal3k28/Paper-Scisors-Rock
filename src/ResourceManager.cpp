#include <ResourceManager.hpp>

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

ResourceManager::~ResourceManager() {
    this->cleanUp();
}

ResourceManager::ResourceManager() {
    fonts[DEFAULT_FONT] = std::make_shared<Font>( GetFontDefault() );

    // Default texture
    Image image = GenImageColor(64, 64, BLACK); 
    textures[DEFAULT_TEXTURE] = std::make_shared<Texture2D>( LoadTextureFromImage( image ) );
    // Unload image
    UnloadImage(image);
}

void ResourceManager::loadTexture(const std::string& key, const std::string& filePath) {
    if (textures.find(key) == textures.end()) {
        textures[key] = std::make_shared<Texture2D>( LoadTexture(filePath.c_str() ) );
    }
}

std::shared_ptr<Texture2D> ResourceManager::getTexture(const std::string& key) {
    if ( textures.find(key) != textures.end() ) {
        return textures.at(key);
    }
    
    // Return default texture
    return textures.at(DEFAULT_TEXTURE);
}

void ResourceManager::unloadTexture(const std::string& key) {
    if (textures.find(key) != textures.end()) {
        if (textures[key] != nullptr)
            UnloadTexture(*textures[key]);
        textures.erase(key);
    }
}

void ResourceManager::loadSound(const std::string& key, const std::string& filePath) {
    if (sounds.find(key) == sounds.end()) {
        sounds[key] = std::make_shared<Sound>( LoadSound( filePath.c_str() ) );
    }
}

std::shared_ptr<Sound> ResourceManager::getSound(const std::string& key) {
    return sounds.at(key);
}

void ResourceManager::unloadSound(const std::string& key) {
    if (sounds.find(key) != sounds.end()) {
        UnloadSound(*sounds[key]);
        sounds.erase(key);
    }
}

void ResourceManager::loadFont(const std::string & key, const std::string & filePath) {
    if (fonts.find(key) == fonts.end()) {
        fonts[key] = std::make_shared<Font>( LoadFont( filePath.c_str() ) );
    }
}

std::shared_ptr<Font> ResourceManager::getFont(const std::string &key) {
    if (fonts.find(key) != fonts.end()) {
        return fonts.at(key);
    }
    return fonts.at(DEFAULT_FONT);
}

void ResourceManager::unloadFont(const std::string &key) {
    if (fonts.find(key) != fonts.end()) {
        UnloadFont(*fonts[key]);
        fonts.erase(key);
    }
}

void ResourceManager::cleanUp() {
    for (auto& [key, texture] : textures) {
        UnloadTexture(*texture);
    }
    textures.clear();

    for (auto& [key, sound] : sounds) {
        UnloadSound(*sound);
    }
    sounds.clear();

    for (auto& [key, font] : fonts) {
        UnloadFont(*font);
    }
    fonts.clear();
}
