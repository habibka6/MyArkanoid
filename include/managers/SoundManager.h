#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

namespace Arkanoid {

    enum class SoundType {
        BlockHit,
        RockHit,
        PaddleHit,
        WallHit,
        GameOver,
        LevelComplete,
        PowerUpCollect,
        ButtonClick
    };

    class SoundManager {
    public:
        static SoundManager& getInstance();

        // Воспроизведение звуков
        void playSound(SoundType soundType);
        void playSound(const std::string& filename);

        // Управление громкостью
        void setSoundVolume(float volume);
        void setMusicVolume(float volume);
        float getSoundVolume() const;
        float getMusicVolume() const;

        // Управление музыкой
        void playMusic(const std::string& filename, bool loop = true);
        void pauseMusic();
        void stopMusic();
        void resumeMusic();
        bool isMusicPlaying() const;

        // Глобальные настройки
        void setMuted(bool muted);
        bool isMuted() const;

        // Очистка ресурсов
        void cleanup();

        // Удаление копирования
        SoundManager(const SoundManager&) = delete;
        SoundManager& operator=(const SoundManager&) = delete;

    private:
        SoundManager();
        ~SoundManager() = default;

        // Пулы звуков для избежания прерывания
        std::vector<std::unique_ptr<sf::Sound>> soundPool;
        std::unordered_map<SoundType, std::string> soundMap;

        sf::Music music;
        float soundVolume;
        float musicVolume;
        bool muted;

        // Константы
        static const size_t MAX_CONCURRENT_SOUNDS = 10;

        // Вспомогательные методы
        void initializeSoundMap();
        sf::Sound* getAvailableSound();
        std::string getSoundFilename(SoundType soundType) const;
    };

} // namespace Arkanoid
