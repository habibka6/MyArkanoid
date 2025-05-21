#pragma once
#include <SFML/Audio.hpp>
#include "AssetManager.h"

class SoundManager {
public:
    SoundManager();

    sf::Sound hitPaddleSound;
    sf::Sound hitBlockSound;
    sf::Sound hitRockSound;
    sf::Sound loseBallSound;
};