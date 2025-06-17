#pragma once
#include "Entity.h"
// Типы коллизий для уведомлений
namespace Arkanoid {
    enum class CollisionType {
        BallBlock,
        BallPaddle,
        BallWall,
        PaddlePowerUp
    };

    // Интерфейс наблюдателя за коллизиями (паттерн Observer)
    class ICollisionObserver {
    public:
        virtual ~ICollisionObserver() = default;
        virtual void onCollision(CollisionType type, Entity* obj1, Entity* obj2) = 0; // Обработка коллизии
    };

}
