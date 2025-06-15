#pragma once
#include "Ball.h"
#include "Paddle.h"
#include "BaseBlock.h"
#include "PowerUp.h"
#include "SpatialGrid.h"
#include "BlockCollisionSolver.h"
#include "PaddleCollisionSolver.h"
#include "WallCollisionSolver.h"
#include "ICollisionObserver.h" 
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

namespace Arkanoid {

    class PhysicsSystem {
    public:
        PhysicsSystem(sf::RenderWindow& window);
        ~PhysicsSystem() = default;

        void update(Ball& ball, Paddle& paddle,
            std::vector<std::unique_ptr<BaseBlock>>& blocks,
            std::vector<std::unique_ptr<PowerUp>>& powerups,
            float deltaTime);

        // Observer methods
        void addObserver(ICollisionObserver* observer);
        void removeObserver(ICollisionObserver* observer);

        void setWorldBounds(const sf::FloatRect& bounds);
        bool isBallLost(const Ball& ball) const;

    private:
        sf::RenderWindow& window;
        SpatialGrid spatialGrid;
        sf::FloatRect worldBounds;

        std::unique_ptr<BlockCollisionSolver> blockSolver;
        std::unique_ptr<PaddleCollisionSolver> paddleSolver;
        std::unique_ptr<WallCollisionSolver> wallSolver;

        // Observer storage
        std::vector<ICollisionObserver*> observers;

        // Notify all observers
        void notifyObservers(CollisionType type, Entity* obj1, Entity* obj2);

        void checkBallCollisions(Ball& ball, Paddle& paddle,
            std::vector<std::unique_ptr<BaseBlock>>& blocks,
            float deltaTime);
        void checkPowerUpCollisions(std::vector<std::unique_ptr<PowerUp>>& powerups,
            Paddle& paddle);
        void checkWallCollisions(Ball& ball, float deltaTime);
        void constrainPaddleToWindow(Paddle& paddle);
    };

}
