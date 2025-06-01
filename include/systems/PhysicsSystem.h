#pragma once
#include "Ball.h"
#include "Paddle.h"
#include "BaseBlock.h"
#include "BasePowerUp.h"
#include "SpatialGrid.h"
#include "BlockCollisionSolver.h"
#include "PaddleCollisionSolver.h"
#include "WallCollisionSolver.h"
#include "PhysicsUtils.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <functional>

namespace Arkanoid {

    // Типы коллизий для уведомлений
    enum class CollisionType {
        BallBlock,
        BallPaddle,
        BallWall,
        PaddlePowerUp,
        BallLost
    };

    // Функция обратного вызова для обработки коллизий
    using CollisionCallback = std::function<void(CollisionType, Entity*, Entity*)>;

    class PhysicsSystem {
    public:
        PhysicsSystem(sf::RenderWindow& window);
        ~PhysicsSystem() = default;

        // Основной метод обновления физики
        void update(Ball& ball, Paddle& paddle,
            std::vector<std::unique_ptr<BaseBlock>>& blocks,
            std::vector<std::unique_ptr<BasePowerUp>>& powerups,
            float deltaTime);

        // Регистрация колбэков для обработки коллизий
        void setCollisionCallback(CollisionCallback callback);

        // Настройка границ мира
        void setWorldBounds(const sf::FloatRect& bounds);

        // Проверка потери мяча
        bool isBallLost(const Ball& ball) const;

    private:
        sf::RenderWindow& window;
        SpatialGrid spatialGrid;
        CollisionCallback collisionCallback;
        sf::FloatRect worldBounds;

        // Солверы коллизий
        std::unique_ptr<BlockCollisionSolver> blockSolver;
        std::unique_ptr<PaddleCollisionSolver> paddleSolver;
        std::unique_ptr<WallCollisionSolver> wallSolver;

        // Методы обнаружения коллизий
        void checkBallCollisions(Ball& ball, Paddle& paddle,
            std::vector<std::unique_ptr<BaseBlock>>& blocks,
            float deltaTime);
        void checkPowerUpCollisions(std::vector<std::unique_ptr<BasePowerUp>>& powerups,
            Paddle& paddle);
        void checkWallCollisions(Ball& ball, float deltaTime);

        // Обработчики конкретных коллизий
        void handleBallBlockCollision(Ball& ball, BaseBlock& block, float deltaTime);
        void handleBallPaddleCollision(Ball& ball, Paddle& paddle, float deltaTime);
        void handlePaddlePowerUpCollision(Paddle& paddle, BasePowerUp& powerup);

        // Утилиты
        void constrainPaddleToWindow(Paddle& paddle);
        void updatePowerUps(std::vector<std::unique_ptr<BasePowerUp>>& powerups, float deltaTime);
        void removeInactivePowerUps(std::vector<std::unique_ptr<BasePowerUp>>& powerups);
    };

} // namespace Arkanoid
