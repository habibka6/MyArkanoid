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

    // �������
    Paddle paddle;
    Ball ball;
    std::vector<std::unique_ptr<BaseBlock>> blocks;
    SpatialGrid spatialGrid;

    // ��������� ����
    int score = 0;
    int lives = 3;
    bool gameOver = false;
    bool waitingLaunch = true;

    // ������
    void loadLevel();
    void resetBallPosition();
    void launchBall();

    // �������
    float getWindowWidth() const { return windowWidth; }
    float getWindowHeight() const { return windowHeight; }

private:

    float windowWidth;
    float windowHeight;

    void generateLevel();
};