#pragma once
#include "CollisionSolver.h"
#include "PhysicsUtils.h"

namespace Arkanoid {
    class WallCollisionSolver : public CollisionSolver {
    public:
        WallCollisionSolver(float windowWidth, float windowHeight);

        bool checkCollision(Ball& ball, const sf::FloatRect& bounds) const override;
        void resolveCollision(Ball& ball, const sf::FloatRect& bounds, float deltaTime) override;

        bool checkWallCollisions(Ball& ball) const;
        bool isBallLost(const Ball& ball) const;

    private:
        sf::FloatRect worldBounds;
    };
}