#pragma once
#include "SpatialGrid.h"
#include "Ball.h"
#include "Paddle.h"
#include "Block.h"
class PhysicsEngine {
public:
    PhysicsEngine(
        SpatialGrid& spatialGrid,
        sf::Sound& blockSound,
        sf::Sound& rockSound,
        int& score
    );

    void update(
        Ball& ball,
        Paddle& paddle,
        std::vector<std::unique_ptr<BaseBlock>>& blocks,
        float dt,
        const sf::RenderWindow& window
    );

private:
    SpatialGrid& spatialGrid;
    sf::Sound& blockSound;
    sf::Sound& rockSound;
    int& score;
};