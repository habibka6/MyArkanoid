#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include "AssetManager.h"

class AudioManager {
public:
    explicit AudioManager(AssetManager& assetManager);
    void playSound(const std::string& name);

private:
    AssetManager& assetManager;
    std::vector<sf::Sound> activeSounds;
};