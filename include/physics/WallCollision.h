#pragma once
#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "Config.h"
#include <algorithm>
#include <iostream> // ��� �������
#include <random>
class WallCollision {
public:
    static void checkWallCollisions(Ball& ball, const sf::RenderWindow& window);
};
