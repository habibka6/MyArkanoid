#include "AssetManager.h"
#include <iostream>

namespace Arkanoid {

    AssetManager& AssetManager::getInstance() {
        static AssetManager instance;
        return instance;
    }

    template<typename T>
    T& AssetManager::getResource(std::unordered_map<std::string, T>& container,
        const std::string& filename,
        const std::string& path) {
        auto it = container.find(filename);
        if (it != container.end()) {
            return it->second;
        }
        T resource;
        if (loadResource(resource, path + filename)) {
            auto result = container.emplace(filename, std::move(resource));
            return result.first->second;
        }
        else {
            throw std::runtime_error("Failed to load resource: " + path + filename);
        }
    }

    template<typename T>
    bool AssetManager::loadResource(T& resource, const std::string& filepath) {
        return resource.loadFromFile(filepath);
    }

    sf::Texture& AssetManager::getTexture(const std::string& filename) {
        return getResource(textures, filename, TEXTURE_PATH);
    }

    sf::Font& AssetManager::getFont(const std::string& filename) {
        return getResource(fonts, filename, FONT_PATH);
    }

    sf::SoundBuffer& AssetManager::getSoundBuffer(const std::string& filename) {
        return getResource(soundBuffers, filename, SOUND_PATH);
    }

    sf::Image& AssetManager::getImage(const std::string& filename) {
        return getResource(images, filename, IMAGE_PATH);
    }

    bool AssetManager::preloadTextures() {
        try {
            getTexture("ball.png");
            getTexture("paddle.png");
            getTexture("background.png");
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Error preloading textures: " << e.what() << std::endl;
            return false;
        }
    }

    bool AssetManager::preloadFonts() {
        try {
            getFont("retro.ttf");
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Error preloading fonts: " << e.what() << std::endl;
            return false;
        }
    }

    bool AssetManager::preloadSounds() {
        try {
            getSoundBuffer("block_hit.wav");
            getSoundBuffer("paddle_hit.wav");
            getSoundBuffer("wall_hit.wav");
            getSoundBuffer("game_over.wav");
            getSoundBuffer("level_complete.wav");
            getSoundBuffer("powerup_collect.wav");
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Error preloading sounds: " << e.what() << std::endl;
            return false;
        }
    }

    bool AssetManager::preloadAll() {
        return preloadTextures() && preloadFonts() && preloadSounds();
    }

    void AssetManager::clear() {
        clearTextures();
        clearFonts();
        clearSounds();
        clearImages();
    }

    void AssetManager::clearTextures() {
        textures.clear();
    }

    void AssetManager::clearFonts() {
        fonts.clear();
    }

    void AssetManager::clearSounds() {
        soundBuffers.clear();
    }

    void AssetManager::clearImages() {
        images.clear();
    }

    bool AssetManager::hasTexture(const std::string& filename) const {
        return textures.find(filename) != textures.end();
    }

    bool AssetManager::hasFont(const std::string& filename) const {
        return fonts.find(filename) != fonts.end();
    }

    bool AssetManager::hasSoundBuffer(const std::string& filename) const {
        return soundBuffers.find(filename) != soundBuffers.end();
    }

    bool AssetManager::hasImage(const std::string& filename) const {
        return images.find(filename) != images.end();
    }

} 
