#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>

//Класс для управления ресурсами игры (текстуры,шрифты,звуки,изображения)
class AssetManager {
public:
    //Методы для загрузки и получения ресурсов
    static sf::Texture& GetTexture(const std::string& filename);
    static sf::Font& GetFont(const std::string& filename);
    static sf::SoundBuffer& GetSoundBuffer(const std::string& filename);
    static sf::Image& GetImage(const std::string& filename);

private:
    //Контейнеры для хранения ресурсов
    static std::map<std::string, sf::Texture> textures;
    static std::map<std::string, sf::Font> fonts;
    static std::map<std::string, sf::SoundBuffer> soundBuffers;
    static std::map<std::string, sf::Image> images;
};