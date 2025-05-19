#pragma once
#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "Block.h"
#include "Config.h"
#include <algorithm>
#include <iostream> // Для отладки
#include <random>
#include "SpatialGrid.h"
class BlockCollision {
public:
   
    static void checkBlockCollisions(Ball& ball, std::vector<std::unique_ptr<BaseBlock>>& blocks, int& score, float dt, sf::Sound& sound, sf::Sound& sound2, SpatialGrid& spatialGrid);
     // Возвращает пару: (нормаль столкновения, tEntry). 
    // Если нормаль == (0,0), tEntry игнорируется.
    static std::pair<sf::Vector2f, float> sweepAABB(const sf::Vector2f& startPos, 
        const sf::Vector2f& velocity, float dt, const sf::FloatRect& box, float radius);

};
