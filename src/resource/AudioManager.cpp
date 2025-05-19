#include "AudioManager.h"

AudioManager::AudioManager(AssetManager& assetManager)
    : assetManager(assetManager) {
}

void AudioManager::playSound(const std::string& name) {
    const sf::SoundBuffer& buffer = assetManager.GetSoundBuffer(name);
    activeSounds.emplace_back(buffer);
    activeSounds.back().play();

    // Удаление завершенных звуков
    activeSounds.erase(
        std::remove_if(activeSounds.begin(), activeSounds.end(),
            [](const sf::Sound& s) { return s.getStatus() == sf::Sound::Stopped; }),
        activeSounds.end()
    );
}