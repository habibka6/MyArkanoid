#pragma once
#include "CollisionSolver.h"
#include "BaseBlock.h"
#include "PhysicsUtils.h"

namespace Arkanoid {
    class BlockCollisionSolver : public CollisionSolver {
    public:
        bool checkCollision(Ball& ball, const sf::FloatRect& bounds) const override;
        void resolveCollision(Ball& ball, const sf::FloatRect& bounds, float deltaTime) override;

        bool checkBlockCollision(Ball& ball, BaseBlock& block) const;
        void resolveBlockCollision(Ball& ball, BaseBlock& block, float deltaTime);
    };
}

