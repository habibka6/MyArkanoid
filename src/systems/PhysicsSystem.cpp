#include "PhysicsSystem.h"
#include "Config.h"
#include <algorithm>

namespace Arkanoid {

    PhysicsSystem::PhysicsSystem(sf::RenderWindow& window)
        : window(window),
        spatialGrid(Config::Window::WIDTH, Config::Window::HEIGHT, 100.0f) {

        worldBounds = sf::FloatRect(0, 0, Config::Window::WIDTH, Config::Window::HEIGHT);

        // Инициализируем солверы коллизий
        blockSolver = std::make_unique<BlockCollisionSolver>();
        paddleSolver = std::make_unique<PaddleCollisionSolver>();
        wallSolver = std::make_unique<WallCollisionSolver>(Config::Window::WIDTH, Config::Window::HEIGHT);
    }

    void PhysicsSystem::update(Ball& ball, Paddle& paddle,
        std::vector<std::unique_ptr<BaseBlock>>& blocks,
        std::vector<std::unique_ptr<BasePowerUp>>& powerups,
        float deltaTime) {

        // Обновляем пространственную сетку
        spatialGrid.update(blocks);

        // Ограничиваем движение платформы
        constrainPaddleToWindow(paddle);

        // Проверяем коллизии мяча
        checkBallCollisions(ball, paddle, blocks, deltaTime);

        // Проверяем коллизии со стенами
        checkWallCollisions(ball, deltaTime);

        // Обновляем и проверяем коллизии бонусов
        
        checkPowerUpCollisions(powerups, paddle);

        // Проверяем, не потерян ли мяч
        if (isBallLost(ball)) {
            if (collisionCallback) {
                collisionCallback(CollisionType::BallLost, &ball, nullptr);
            }
        }
    }

    void PhysicsSystem::checkBallCollisions(Ball& ball, Paddle& paddle,
        std::vector<std::unique_ptr<BaseBlock>>& blocks,
        float deltaTime) {
        sf::FloatRect ballBounds = ball.getBounds();

        // Коллизия с платформой (проверяем первой для приоритета)
        if (paddleSolver->checkPaddleCollision(ball, paddle)) {
            paddleSolver->resolvePaddleCollision(ball, paddle, deltaTime);

            if (collisionCallback) {
                collisionCallback(CollisionType::BallPaddle, &ball, &paddle);
            }
            return; // Обрабатываем только одну коллизию за кадр
        }

        // Коллизии с блоками (используем пространственную оптимизацию)
        auto potentialBlocks = spatialGrid.getPotentialCollisions(ballBounds);
        for (auto* block : potentialBlocks) {
            if (blockSolver->checkBlockCollision(ball, *block)) {
                blockSolver->resolveBlockCollision(ball, *block, deltaTime);

                if (collisionCallback) {
                    collisionCallback(CollisionType::BallBlock, &ball, &*block);
                }
                break; // Обрабатываем только одну коллизию за кадр
            }
        }
    }

    void PhysicsSystem::checkPowerUpCollisions(std::vector<std::unique_ptr<BasePowerUp>>& powerups,
        Paddle& paddle) {
        sf::FloatRect paddleBounds = paddle.getBounds();

        for (auto& powerup : powerups) {
            if (powerup->isActive() && paddleBounds.intersects(powerup->getBounds())) {
                powerup->setActive(false);

                if (collisionCallback) {
                    collisionCallback(CollisionType::PaddlePowerUp, &paddle, &*powerup);
                }
            }
        }
    }

    void PhysicsSystem::checkWallCollisions(Ball& ball, float deltaTime) {
        if (wallSolver->checkWallCollisions(ball)) {
            wallSolver->resolveCollision(ball, sf::FloatRect(), deltaTime);

            if (collisionCallback) {
                collisionCallback(CollisionType::BallWall, &ball, nullptr);
            }
        }
    }

    void PhysicsSystem::constrainPaddleToWindow(Paddle& paddle) {
        paddle.constrainToWindow(worldBounds.width);
    }


    bool PhysicsSystem::isBallLost(const Ball& ball) const {
        return wallSolver->isBallLost(ball);
    }

    void PhysicsSystem::setCollisionCallback(CollisionCallback callback) {
        collisionCallback = callback;
    }

    void PhysicsSystem::setWorldBounds(const sf::FloatRect& bounds) {
        worldBounds = bounds;
        wallSolver = std::make_unique<WallCollisionSolver>(bounds.width, bounds.height);
    }

} // namespace Arkanoid
