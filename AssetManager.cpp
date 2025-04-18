#include "AssetManager.h"

//������������� ����������� �����������
std::map<std::string, sf::Texture> AssetManager::textures;
std::map<std::string, sf::Font> AssetManager::fonts;
std::map<std::string, sf::SoundBuffer> AssetManager::soundBuffers;
std::map<std::string, sf::Image> AssetManager::images;

//�������� �������� �� �����
sf::Texture& AssetManager::GetTexture(const std::string& filename) {
    if (!textures[filename].loadFromFile("assets/textures/" + filename)) {
        throw std::runtime_error("Failed to load texture: " + filename);
    }
    return textures[filename];
}

//�������� ������ �� �����
sf::Font& AssetManager::GetFont(const std::string& filename) {
    if (!fonts[filename].loadFromFile("assets/fonts/" + filename)) {
        throw std::runtime_error("Failed to load font: " + filename);
    }
    return fonts[filename];
}

//�������� ��������� ������ �� �����
sf::SoundBuffer& AssetManager::GetSoundBuffer(const std::string& filename) {
    if (!soundBuffers[filename].loadFromFile("assets/sounds/" + filename)) {
        throw std::runtime_error("Failed to load sound: " + filename);
    }
    return soundBuffers[filename];
}

//�������� ����������� �� �����
sf::Image& AssetManager::GetImage(const std::string& filename) {
    if (!images[filename].loadFromFile("assets/images/" + filename)) {
        throw std::runtime_error("Failed to load image: " + filename);
    }
    return images[filename];
}