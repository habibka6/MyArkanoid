#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>

namespace Arkanoid {

    class AssetManager {
    public:
        static AssetManager& getInstance();

        // �������� � ��������� ��������
        sf::Texture& getTexture(const std::string& filename);
        sf::Font& getFont(const std::string& filename);
        sf::SoundBuffer& getSoundBuffer(const std::string& filename);
        sf::Image& getImage(const std::string& filename);

        // ������������ ��������
        bool preloadTextures();
        bool preloadFonts();
        bool preloadSounds();
        bool preloadAll();

        // ���������� �������
        void clear();
        void clearTextures();
        void clearFonts();
        void clearSounds();
        void clearImages();

        // �������� ������� ��������
        bool hasTexture(const std::string& filename) const;
        bool hasFont(const std::string& filename) const;
        bool hasSoundBuffer(const std::string& filename) const;
        bool hasImage(const std::string& filename) const;

        // �������� �����������
        AssetManager(const AssetManager&) = delete;
        AssetManager& operator=(const AssetManager&) = delete;

    private:
        AssetManager() = default;
        ~AssetManager() = default;

        // ���������� ��� ��������
        std::unordered_map<std::string, sf::Texture> textures;
        std::unordered_map<std::string, sf::Font> fonts;
        std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
        std::unordered_map<std::string, sf::Image> images;

        // ���� � ��������
        const std::string TEXTURE_PATH = "assets/textures/";
        const std::string FONT_PATH = "assets/fonts/";
        const std::string SOUND_PATH = "assets/sounds/";
        const std::string IMAGE_PATH = "assets/images/";

        // ��������������� ������
        template<typename T>
        T& getResource(std::unordered_map<std::string, T>& container,
            const std::string& filename,
            const std::string& path);

        template<typename T>
        bool loadResource(T& resource, const std::string& filepath);
    };

} // namespace Arkanoid
