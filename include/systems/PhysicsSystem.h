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
    // Система физики, обрабатывающая коллизии и движения объектов
    class PhysicsSystem {
    public:
        PhysicsSystem(sf::RenderWindow& window);
        ~PhysicsSystem() = default;


        void update(Ball& ball, Paddle& paddle,
            std::vector<std::unique_ptr<BaseBlock>>& blocks,
            std::vector<std::unique_ptr<PowerUp>>& powerups,
            float deltaTime);

        // Методы для управления наблюдателями
        void addObserver(ICollisionObserver* observer);
        void removeObserver(ICollisionObserver* observer);

        void setWorldBounds(const sf::FloatRect& bounds);
    private:
        sf::RenderWindow& window;
        SpatialGrid spatialGrid;
        sf::FloatRect worldBounds;

        std::unique_ptr<BlockCollisionSolver> blockSolver;
        std::unique_ptr<PaddleCollisionSolver> paddleSolver;
        std::unique_ptr<WallCollisionSolver> wallSolver;


        std::vector<ICollisionObserver*> observers; // Список наблюдателей

        // Уведомление наблюдателей
        void notifyObservers(CollisionType type, Entity* obj1, Entity* obj2);

        // Методы проверки коллизий
        void checkBallCollisions(Ball& ball, Paddle& paddle,
            std::vector<std::unique_ptr<BaseBlock>>& blocks,
            float deltaTime);
        void checkPowerUpCollisions(std::vector<std::unique_ptr<PowerUp>>& powerups,
            Paddle& paddle);
        void checkWallCollisions(Ball& ball, float deltaTime);

        // Ограничение движения платформы
        void constrainPaddleToWindow(Paddle& paddle);
    };

}
