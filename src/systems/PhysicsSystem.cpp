#include "PhysicsSystem.h"
#include "Config.h"
#include <algorithm>

namespace Arkanoid {

    PhysicsSystem::PhysicsSystem(sf::RenderWindow& window)
        : window(window),
        spatialGrid(Config::Window::WIDTH, Config::Window::HEIGHT, 100.0f) {

        worldBounds = sf::FloatRect(0, 0, Config::Window::WIDTH, Config::Window::HEIGHT);

        blockSolver = std::make_unique<BlockCollisionSolver>();
        paddleSolver = std::make_unique<PaddleCollisionSolver>();
        wallSolver = std::make_unique<WallCollisionSolver>(Config::Window::WIDTH, Config::Window::HEIGHT);
    }


    void PhysicsSystem::addObserver(ICollisionObserver* observer) {
        observers.push_back(observer);
    }

    void PhysicsSystem::removeObserver(ICollisionObserver* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void PhysicsSystem::notifyObservers(CollisionType type, Entity* obj1, Entity* obj2) {
        for (auto* observer : observers) {
            observer->onCollision(type, obj1, obj2);
        }
    }



    void PhysicsSystem::update(Ball& ball, Paddle& paddle,
        std::vector<std::unique_ptr<BaseBlock>>& blocks,
        std::vector<std::unique_ptr<PowerUp>>& powerups,
        float deltaTime) {

        spatialGrid.update(blocks);

        constrainPaddleToWindow(paddle);

        checkBallCollisions(ball, paddle, blocks, deltaTime);

        checkWallCollisions(ball, deltaTime);

        checkPowerUpCollisions(powerups, paddle);

        if (isBallLost(ball)) {
            notifyObservers(CollisionType::BallLost, &ball, nullptr);
        }
    }

    void PhysicsSystem::checkBallCollisions(Ball& ball, Paddle& paddle,
        std::vector<std::unique_ptr<BaseBlock>>& blocks,
        float deltaTime) {
        sf::FloatRect ballBounds = ball.getBounds();

        // Paddle collision
        if (paddleSolver->checkPaddleCollision(ball, paddle)) {
            paddleSolver->resolvePaddleCollision(ball, paddle, deltaTime);

            notifyObservers(CollisionType::BallPaddle, &ball, &paddle);
            return;
        }

        // Block collisions
        auto potentialBlocks = spatialGrid.getPotentialCollisions(ballBounds);
        for (auto* block : potentialBlocks) {
            if (blockSolver->checkBlockCollision(ball, *block)) {
                blockSolver->resolveBlockCollision(ball, *block, deltaTime);

                notifyObservers(CollisionType::BallBlock, &ball, &*block);
                break; 
            }
        }
    }

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

    void PhysicsSystem::checkWallCollisions(Ball& ball, float deltaTime) {
        if (wallSolver->checkWallCollisions(ball)) {
            wallSolver->resolveCollision(ball, sf::FloatRect(), deltaTime);

            notifyObservers(CollisionType::BallWall, &ball, nullptr);
        }
    }

    void PhysicsSystem::constrainPaddleToWindow(Paddle& paddle) {
        paddle.constrainToWindow(worldBounds.width);
    }

    bool PhysicsSystem::isBallLost(const Ball& ball) const {
        return wallSolver->isBallLost(ball,worldBounds.height);
    }

    void PhysicsSystem::setWorldBounds(const sf::FloatRect& bounds) {
        worldBounds = bounds;
        wallSolver = std::make_unique<WallCollisionSolver>(bounds.width, bounds.height);
    }

} // namespace Arkanoid
 