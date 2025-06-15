#pragma once
#include "Entity.h"
// ���� �������� ��� �����������
namespace Arkanoid {
enum class CollisionType {
    BallBlock,
    BallPaddle,
    BallWall,
    PaddlePowerUp,
    BallLost
};

    class ICollisionObserver {
    public:
        virtual ~ICollisionObserver() = default;
        virtual void onCollision(CollisionType type, Entity* obj1, Entity* obj2) = 0;
    };

}
