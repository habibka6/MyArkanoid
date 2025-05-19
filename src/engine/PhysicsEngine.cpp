#include "PhysicsEngine.h"
#include "BlockCollision.h"
#include "PaddleCollision.h"
#include "WallCollision.h"

PhysicsEngine::PhysicsEngine(
    SpatialGrid& spatialGrid,
    sf::Sound& blockSound,
    sf::Sound& rockSound,
    int& score
) : spatialGrid(spatialGrid),
blockSound(blockSound),
rockSound(rockSound),
score(score) {
}

void PhysicsEngine::update(
    Ball& ball,
    Paddle& paddle,
    std::vector<std::unique_ptr<BaseBlock>>& blocks,
    float dt,
    const sf::RenderWindow& window
) {
    // Обновляем SpatialGrid
    spatialGrid.update(blocks);

    // Проверяем коллизии
    PaddleCollision::checkPaddleCollision(ball, paddle, blockSound);
    BlockCollision::checkBlockCollisions(ball, blocks, score, dt, blockSound, rockSound, spatialGrid);
    WallCollision::checkWallCollisions(ball, window); // Передаем окно
}