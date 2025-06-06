#pragma once
#include "BaseBlock.h"
#include "PhysicsUtils.h"

namespace Arkanoid {
    class BlockCollisionSolver {
    public:
        bool checkBlockCollision(Ball& ball, BaseBlock& block) const;
        void resolveBlockCollision(Ball& ball, BaseBlock& block, float deltaTime);
    };
}

