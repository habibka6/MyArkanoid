#pragma once
#include "PhysicsUtils.h"

namespace Arkanoid {
    class WallCollisionSolver{
    public:
        WallCollisionSolver(float windowWidth, float windowHeight);
        void resolveCollision(Ball& ball, const sf::FloatRect& bounds, float deltaTime);

        bool checkWallCollisions(Ball& ball) const;
        bool isBallLost(const Ball& ball) const;

    private:
        sf::FloatRect worldBounds;
    };
}