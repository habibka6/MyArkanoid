#include "PhysicsSystem.h"
#include "Config.h"
#include <algorithm>

namespace Arkanoid {

    PhysicsSystem::PhysicsSystem(sf::RenderWindow& window)
        : window(window),
        spatialGrid(Config::Window::WIDTH, Config::Window::HEIGHT, Config::Block::WIDTH * 5 ) {

        worldBounds = sf::FloatRect(0, 0, Config::Window::WIDTH, Config::Window::HEIGHT);

        blockSolver = std::make_unique<BlockCollisionSolver>();
        paddleSolver = std::make_unique<PaddleCollisionSolver>();
        wallSolver = std::make_unique<WallCollisionSolver>(Config::Window::WIDTH, Config::Window::HEIGHT);
    }

    // Добавить наблюдателя за коллизиями
    void PhysicsSystem::addObserver(ICollisionObserver* observer) {
        observers.push_back(observer);
    }

    // Удалить наблюдателя
    void PhysicsSystem::removeObserver(ICollisionObserver* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    // Оповестить всех наблюдателей о коллизии
    void PhysicsSystem::notifyObservers(CollisionType type, Entity* obj1, Entity* obj2) {
        for (auto* observer : observers) {
            observer->onCollision(type, obj1, obj2);
        }
    }

    // Главный update физики за кадр
    void PhysicsSystem::update(Ball& ball, Paddle& paddle,
        std::vector<std::unique_ptr<BaseBlock>>& blocks,
        std::vector<std::unique_ptr<PowerUp>>& powerups,
        float deltaTime) {

        spatialGrid.update(blocks); 

        constrainPaddleToWindow(paddle); 

        checkBallCollisions(ball, paddle, blocks, deltaTime); 

        checkWallCollisions(ball, deltaTime);

        checkPowerUpCollisions(powerups, paddle); 
    }

    // Проверка и обработка коллизий шарика с платформой и блоками
    void PhysicsSystem::checkBallCollisions(Ball& ball, Paddle& paddle,
        std::vector<std::unique_ptr<BaseBlock>>& blocks,
        float deltaTime) {
        sf::FloatRect ballBounds = ball.getBounds();

        // Коллизия с платформой
        if (paddleSolver->checkPaddleCollision(ball, paddle)) {
            paddleSolver->resolvePaddleCollision(ball, paddle, deltaTime);
            notifyObservers(CollisionType::BallPaddle, &ball, &paddle);
            return;
        }

        // Коллизии с блоками (только ближайшие по spatial grid)
        auto potentialBlocks = spatialGrid.getPotentialCollisions(ballBounds);
        for (auto* block : potentialBlocks) {
            if (blockSolver->checkBlockCollision(ball, *block)) {
                blockSolver->resolveBlockCollision(ball, *block, deltaTime);
                notifyObservers(CollisionType::BallBlock, &ball, &*block);
                break; 
            }
        }
    }

    // Проверка и обработка коллизий бонусов с платформой
    void PhysicsSystem::checkPowerUpCollisions(std::vector<std::unique_ptr<PowerUp>>& powerups,
        Paddle& paddle) {
        sf::FloatRect paddleBounds = paddle.getBounds();

        for (auto& powerup : powerups) {
            if (powerup->isActive() && paddle.getBounds().intersects(powerup->getBounds())) {
                powerup->setActive(false); 
                notifyObservers(CollisionType::PaddlePowerUp, &paddle, &*powerup);
            }
        }
    }

    // Проверка и обработка коллизий шарика со стенами
    void PhysicsSystem::checkWallCollisions(Ball& ball, float deltaTime) {
        if (wallSolver->checkWallCollisions(ball)) {
            wallSolver->resolveCollision(ball, sf::FloatRect(), deltaTime);
            notifyObservers(CollisionType::BallWall, &ball, nullptr);
        }
    }

    // Не даём платформе выйти за пределы окна
    void PhysicsSystem::constrainPaddleToWindow(Paddle& paddle) {
        paddle.constrainToWindow(worldBounds.width);
    }

    // Установка новых границ мира (например, при изменении размера окна)
    void PhysicsSystem::setWorldBounds(const sf::FloatRect& bounds) {
        worldBounds = bounds;
        wallSolver = std::make_unique<WallCollisionSolver>(bounds.width, bounds.height);
    }

} // namespace Arkanoid
