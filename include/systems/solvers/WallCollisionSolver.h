#pragma once
#include "Ball.h"
#include <SFML/Graphics.hpp>

namespace Arkanoid {

    class WallCollisionSolver {
    public:
        WallCollisionSolver(float windowWidth, float windowHeight);
        void resolveCollision(Ball& ball, const sf::FloatRect& bounds, float deltaTime);
        bool checkWallCollisions(Ball& ball) const;
    private:
        sf::FloatRect worldBounds; // Границы игрового поля
    };

}
