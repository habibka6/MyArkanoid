#pragma once
#include "GameState.h"
#include "BlockCollision.h"
#include "PaddleCollision.h"
#include "WallCollision.h"
#include <SFML/Audio.hpp>

class CollisionManager {
public:
    CollisionManager(GameState& state, const sf::RenderWindow& window, sf::Sound& paddleSound, sf::Sound& blockSound, sf::Sound& rockSound);
    void checkCollisions(float dt);

private:
    GameState& state;
    const sf::RenderWindow& window; // Добавлена ссылка на окно
    sf::Sound& hitPaddleSound;
    sf::Sound& hitBlockSound;
    sf::Sound& hitRockSound;
};