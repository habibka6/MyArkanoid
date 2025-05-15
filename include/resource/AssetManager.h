#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>

class AssetManager {
public:
    static AssetManager& getInstance();

    sf::Texture& GetTexture(const std::string& filename);
    sf::Font& GetFont(const std::string& filename);
    sf::SoundBuffer& GetSoundBuffer(const std::string& filename);
    sf::Image& GetImage(const std::string& filename);

    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

private:
    AssetManager() = default;

    static std::map<std::string, sf::Texture> textures;
    static std::map<std::string, sf::Font> fonts;
    static std::map<std::string, sf::SoundBuffer> soundBuffers;
    static std::map<std::string, sf::Image> images;
};