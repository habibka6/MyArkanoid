#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>

// Класс-менеджер ресурсов (синглтон). 
// Обеспечивает загрузку и хранение текстур, шрифтов, звуков и изображений.
class AssetManager {
public:
    // Получение экземпляра менеджера (паттерн синглтон)
    static AssetManager& getInstance();

    // Загрузка и доступ к текстуре по имени файла
    sf::Texture& GetTexture(const std::string& filename);
    // Загрузка и доступ к шрифту по имени файла
    sf::Font& GetFont(const std::string& filename);
    // Загрузка и доступ к звуковому буферу по имени файла
    sf::SoundBuffer& GetSoundBuffer(const std::string& filename);
    // Загрузка и доступ к изображению по имени файла
    sf::Image& GetImage(const std::string& filename);

    // Удаление конструкторов копирования и присваивания (синглтон)
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

private:
    AssetManager() = default; // Приватный конструктор

    // Контейнеры для хранения ресурсов:
    static std::map<std::string, sf::Texture> textures; // Текстуры
    static std::map<std::string, sf::Font> fonts; // Шрифты
    static std::map<std::string, sf::SoundBuffer> soundBuffers; // Звуки
    static std::map<std::string, sf::Image> images;   // Изображения
};