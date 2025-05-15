#pragma once
#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "Paddle.h"
#include "Block.h"
#include "Config.h"
class Physics {
public:
    static void checkPaddleCollision(Ball& ball, const Paddle& paddle, sf::Sound& sound);
    static void checkBlockCollisions(Ball& ball, std::vector<std::unique_ptr<BaseBlock>>& blocks, int& score, float dt, sf::Sound& sound, sf::Sound& sound2);
    static void checkWallCollisions(Ball& ball, const sf::RenderWindow& window);
    static void checkDeathZone(Ball& ball, int& lives, bool& gameOver, const sf::RenderWindow& window, sf::Sound& sound);
    // Возвращает пару: (нормаль столкновения, tEntry). 
    // Если нормаль == (0,0), tEntry игнорируется.
    static std::pair<sf::Vector2f, float> sweepAABB(
        const sf::Vector2f& startPos,
        const sf::Vector2f& velocity,
        float dt,
        const sf::FloatRect& box,
        float radius
    );
    
};
