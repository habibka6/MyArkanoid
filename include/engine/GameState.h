#pragma once
#include "Paddle.h"
#include "Ball.h"
#include "BaseBlock.h"
#include "Block.h"
#include "SpatialGrid.h"
#include "Config.h"
#include <vector>
#include <memory>
#include <unordered_set>
class GameState {
public:
    explicit GameState(float width, float height);

    // Объекты
    SpatialGrid spatialGrid;

    // Состояние игры
    int score = 0;
    int lives = 3;
    bool gameOver = false;
    bool waitingLaunch = true;

    // Методы
    void loadLevel();
    void resetBallPosition();
    void launchBall();

    // Геттеры
    // Геттеры
    const Paddle& getPaddle() const { return paddle; }
    Paddle& getPaddle() { return paddle; }

    const Ball& getBall() const { return ball; }
    Ball& getBall() { return ball; }

    const std::vector<std::unique_ptr<BaseBlock>>& getBlocks() const { return blocks; }
    std::vector<std::unique_ptr<BaseBlock>>& getBlocks() { return blocks; }

    float getWindowWidth() const { return windowWidth; }
    float getWindowHeight() const { return windowHeight; }

private:
    Paddle paddle;
    Ball ball;
    std::vector<std::unique_ptr<BaseBlock>> blocks;

    float windowWidth;
    float windowHeight;

    void generateLevel();
};