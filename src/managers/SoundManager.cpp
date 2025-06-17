#include "SoundManager.h"
#include "AssetManager.h"
#include <algorithm>

namespace Arkanoid {

    // Конструктор: инициализация звуков и пула
    SoundManager::SoundManager()
        : soundVolume(100.0f),
        musicVolume(50.0f),
        muted(false) {
        initializeSoundMap();
        for (size_t i = 0; i < MAX_CONCURRENT_SOUNDS; ++i) {
            soundPool.push_back(std::make_unique<sf::Sound>());
        }
    }

    // Получение синглтона
    SoundManager& SoundManager::getInstance() {
        static SoundManager instance;
        return instance;
    }

    // Заполнение карты звуков
    void SoundManager::initializeSoundMap() {
        soundMap[SoundType::BlockHit] = "block_hit.wav";
        soundMap[SoundType::RockHit] = "rock_hit.wav";
        soundMap[SoundType::PaddleHit] = "paddle_hit.wav";
        soundMap[SoundType::WallHit] = "wall_hit.wav";
        soundMap[SoundType::GameOver] = "game_over.wav";
        soundMap[SoundType::LevelComplete] = "level_complete.wav";
        soundMap[SoundType::PowerUpCollect] = "powerup_collect.wav";
        soundMap[SoundType::ButtonClick] = "button_click.wav";
        soundMap[SoundType::Error] = "error_sound.wav";
        soundMap[SoundType::LoseBall] = "lose_ball.wav";
    }

    void SoundManager::playSound(SoundType soundType) {
        if (muted) return;
        std::string filename = getSoundFilename(soundType);
        if (!filename.empty()) {
            playSound(filename);
        }
    }

    void SoundManager::playSound(const std::string& filename) {
        if (muted) return;
        try {
            sf::Sound* sound = getAvailableSound();
            if (sound) {
                sf::SoundBuffer& buffer = AssetManager::getInstance().getSoundBuffer(filename);
                sound->setBuffer(buffer);
                sound->setVolume(soundVolume);
                sound->play();
            }
        }
        catch (const std::exception& e) {}
    }

    // Получение свободного звука из пула
    sf::Sound* SoundManager::getAvailableSound() {
        for (auto& sound : soundPool) {
            if (sound->getStatus() != sf::Sound::Playing) {
                return sound.get();
            }
        }
        return soundPool[0].get();
    }


    std::string SoundManager::getSoundFilename(SoundType soundType) const {
        auto it = soundMap.find(soundType);
        return (it != soundMap.end()) ? it->second : "";
    }

    void SoundManager::setSoundVolume(float volume) {
        soundVolume = std::clamp(volume, 0.0f, 100.0f);
    }

    void SoundManager::setMusicVolume(float volume) {
        musicVolume = std::clamp(volume, 0.0f, 100.0f);
        music.setVolume(muted ? 0.0f : musicVolume);
    }

    float SoundManager::getSoundVolume() const {
        return soundVolume;
    }

    float SoundManager::getMusicVolume() const {
        return musicVolume;
    }

    void SoundManager::playMusic(const std::string& filename, bool loop) {
        if (music.openFromFile("assets/music/" + filename)) {
            music.setLoop(loop);
            music.setVolume(muted ? 0.0f : musicVolume);
            music.play();
        }
    }

    void SoundManager::pauseMusic() {
        music.pause();
    }

    void SoundManager::stopMusic() {
        music.stop();
    }

    void SoundManager::resumeMusic() {
        if (!muted) {
            music.play();
        }
    }

    bool SoundManager::isMusicPlaying() const {
        return music.getStatus() == sf::Music::Playing;
    }

    void SoundManager::setMuted(bool muted) {
        this->muted = muted;
        music.setVolume(muted ? 0.0f : musicVolume);
        if (muted) {
            for (auto& sound : soundPool) {
                if (sound->getStatus() == sf::Sound::Playing) {
                    sound->stop();
                }
            }
        }
    }

    bool SoundManager::isMuted() const {
        return muted;
    }

    void SoundManager::cleanup() {
        stopMusic();
        for (auto& sound : soundPool) {
            sound->stop();
        }
    }

} // namespace Arkanoid
