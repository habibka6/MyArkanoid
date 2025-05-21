#include "GameState.h"
#include "Rock.h"
#include <random>
#include <numeric>

GameState::GameState(float width, float height)
    : windowWidth(width), windowHeight(height),
    paddle(width / 2 - 100, height - 50),
    ball(width / 2, height - 100),
    spatialGrid(width, height, SPATIAL_GRID_CELL_SIZE) {
    loadLevel();
}

void GameState::loadLevel() {
    blocks.clear();
    generateLevel();
    spatialGrid.clear();
    for (auto& block : blocks) {
        spatialGrid.addBlock(block.get());
    }
}

void GameState::resetBallPosition() {
    // ����� ���������
    float px = paddle.getPosition().x + paddle.getSize().x / 2;
    float py = paddle.getPosition().y;

    // ������������� ������� ����
    sf::Vector2f newPos(px, py - ball.getBounds().height);
    ball.getSprite().setPosition(newPos);
    ball.savePrevPosition(); // ��������� ������� ��� ������������
}
void GameState::launchBall() {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(-40.f, 40.f);
    float angleDeg = dist(gen) * 3.1415926f / 180.f;

    ball.resetSpeedFactor();
    float speed = ball.getSpeed();
    ball.setVelocity(
        std::sin(angleDeg) * speed,
        -std::cos(angleDeg) * speed
    );
    waitingLaunch = false;
}

void GameState::generateLevel() {
   
    const int cols = 10, rows = 5;
    const float blockW = 90.f, blockH = 40.f;
    const float spacing = 30.f;
    const float marginX = (windowWidth - (cols * blockW + (cols - 1) * spacing)) / 2;
    const float marginY = 60.f;

    // 2) ������� ����� ������ � ��� �������
    const int rockCount = 5;
    std::vector<int> allIdx(cols * rows);
    std::iota(allIdx.begin(), allIdx.end(), 0);

    // 3) ������������
    std::mt19937 gen(std::random_device{}());
    std::shuffle(allIdx.begin(), allIdx.end(), gen);

    // 4) �������� ������� ��� ������ ��� �������
    std::unordered_set<int> rockSet;
    auto hasNeighbor = [&](int idx) {
        int x = idx % cols, y = idx / cols;
        // ��������� 4 �������
        if (x > 0 && rockSet.count(idx - 1))      return true;
        if (x < cols - 1 && rockSet.count(idx + 1))      return true;
        if (y > 0 && rockSet.count(idx - cols))   return true;
        if (y < rows - 1 && rockSet.count(idx + cols))   return true;
        return false;
        };

    for (int idx : allIdx) {
        if ((int)rockSet.size() >= rockCount)
            break;
        if (!hasNeighbor(idx)) {
            rockSet.insert(idx);
        }
    }
    // ���� �� ������� ������ ���������� (������ ������������), 
    // ����� ������ �������� �� ����������:
    for (int idx : allIdx) {
        if ((int)rockSet.size() >= rockCount)
            break;
        rockSet.insert(idx);
    }

    // 5) ����������� ��� ������� ������
    std::discrete_distribution<> colorDist{ 45, 25, 20 };

    // 6) ������ ������ unique_ptr<BlockBase>
    for (int idx = 0; idx < cols * rows; ++idx) {
        int y = idx / cols;
        int x = idx % cols;
        float posX = marginX + x * (blockW + spacing);
        float posY = marginY + y * (blockH + spacing);

        if (rockSet.count(idx)) {
            // �������� ����
            blocks.emplace_back(std::make_unique<Rock>(posX, posY));
        }
        else {
            // ������� �������
            int d = colorDist(gen);
            Block::Type type = (d == 0 ? Block::Type::Green
                : d == 1 ? Block::Type::Yellow
                : Block::Type::Red);
            blocks.emplace_back(std::make_unique<Block>(posX, posY, type));
        }
    }
    
}