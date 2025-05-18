#pragma once
#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "Config.h"
#include <algorithm>
#include <iostream> // Для отладки
#include <random>

class PaddleCollision {
public:
    static void checkPaddleCollision(Ball& ball, const Paddle& paddle, sf::Sound& sound);
    // Если нормаль == (0,0), tEntry игнорируется.
   

};
