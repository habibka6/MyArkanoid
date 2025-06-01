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

    // ���� �������� ��� �����������
    enum class CollisionType {
        BallBlock,
        BallPaddle,
        BallWall,
        PaddlePowerUp,
        BallLost
    };

    // ������� ��������� ������ ��� ��������� ��������
    using CollisionCallback = std::function<void(CollisionType, Entity*, Entity*)>;

    class PhysicsSystem {
    public:
        PhysicsSystem(sf::RenderWindow& window);
        ~PhysicsSystem() = default;

        // �������� ����� ���������� ������
        void update(Ball& ball, Paddle& paddle,
            std::vector<std::unique_ptr<BaseBlock>>& blocks,
            std::vector<std::unique_ptr<BasePowerUp>>& powerups,
            float deltaTime);

        // ����������� �������� ��� ��������� ��������
        void setCollisionCallback(CollisionCallback callback);

        // ��������� ������ ����
        void setWorldBounds(const sf::FloatRect& bounds);

        // �������� ������ ����
        bool isBallLost(const Ball& ball) const;

    private:
        sf::RenderWindow& window;
        SpatialGrid spatialGrid;
        CollisionCallback collisionCallback;
        sf::FloatRect worldBounds;

        // ������� ��������
        std::unique_ptr<BlockCollisionSolver> blockSolver;
        std::unique_ptr<PaddleCollisionSolver> paddleSolver;
        std::unique_ptr<WallCollisionSolver> wallSolver;

        // ������ ����������� ��������
        void checkBallCollisions(Ball& ball, Paddle& paddle,
            std::vector<std::unique_ptr<BaseBlock>>& blocks,
            float deltaTime);
        void checkPowerUpCollisions(std::vector<std::unique_ptr<BasePowerUp>>& powerups,
            Paddle& paddle);
        void checkWallCollisions(Ball& ball, float deltaTime);

        // ����������� ���������� ��������
        void handleBallBlockCollision(Ball& ball, BaseBlock& block, float deltaTime);
        void handleBallPaddleCollision(Ball& ball, Paddle& paddle, float deltaTime);
        void handlePaddlePowerUpCollision(Paddle& paddle, BasePowerUp& powerup);

        // �������
        void constrainPaddleToWindow(Paddle& paddle);
        void updatePowerUps(std::vector<std::unique_ptr<BasePowerUp>>& powerups, float deltaTime);
        void removeInactivePowerUps(std::vector<std::unique_ptr<BasePowerUp>>& powerups);
    };

} // namespace Arkanoid
