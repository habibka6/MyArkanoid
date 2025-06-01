#pragma once
#include "CollisionSolver.h"
#include "Paddle.h"
#include "PhysicsUtils.h"

namespace Arkanoid {
    class PaddleCollisionSolver : public CollisionSolver {
    public:
        bool checkCollision(Ball& ball, const sf::FloatRect& bounds) const override;
        void resolveCollision(Ball& ball, const sf::FloatRect& bounds, float deltaTime) override;

        bool checkPaddleCollision(Ball& ball, Paddle& paddle) const;
        void resolvePaddleCollision(Ball& ball, Paddle& paddle, float deltaTime);
    };
}