#pragma once
#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "Config.h"
#include <algorithm>
#include <iostream> // ��� �������
#include <random>

class PaddleCollision {
public:
    static void checkPaddleCollision(Ball& ball, const Paddle& paddle, sf::Sound& sound);
    // ���� ������� == (0,0), tEntry ������������.
   

};
