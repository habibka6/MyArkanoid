#pragma once
#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "Paddle.h"
#include "Block.h"

class Physics {
public:
    static void checkPaddleCollision(Ball& ball, const Paddle& paddle, sf::Sound& sound);
    static void checkBlockCollisions(Ball& ball, std::vector<Block>& blocks, int& score, sf::Sound& sound);
    static void checkWallCollisions(Ball& ball, const sf::RenderWindow& window);
    static void checkDeathZone(Ball& ball, int& lives, bool& gameOver, const sf::RenderWindow& window, sf::Sound& sound);

};