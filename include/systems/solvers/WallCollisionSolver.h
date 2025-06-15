#pragma once
#include "Ball.h"
#include <SFML/Graphics.hpp>

namespace Arkanoid {

    class WallCollisionSolver {
    public:
        WallCollisionSolver(float windowWidth, float windowHeight);
        void resolveCollision(Ball& ball, const sf::FloatRect& bounds, float deltaTime);
        bool checkWallCollisions(Ball& ball) const;
        static bool isBallLost(const Ball& ball, float worldHeight);

    private:
        sf::FloatRect worldBounds;
    };

}
