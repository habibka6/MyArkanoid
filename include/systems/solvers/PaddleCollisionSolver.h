#pragma once
#include "Paddle.h"
#include "PhysicsUtils.h"

namespace Arkanoid {
    class PaddleCollisionSolver  {
    public:
        bool checkPaddleCollision(Ball& ball, Paddle& paddle) const;
        void resolvePaddleCollision(Ball& ball, Paddle& paddle, float deltaTime);
    };
}