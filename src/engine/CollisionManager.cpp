#include "CollisionManager.h"

CollisionManager::CollisionManager(GameState& state, const sf::RenderWindow& window,
    sf::Sound& paddleSound, sf::Sound& blockSound, sf::Sound& rockSound)
    : state(state),
    window(window), // ������������� ����
    hitPaddleSound(paddleSound),
    hitBlockSound(blockSound),
    hitRockSound(rockSound) {
}

void CollisionManager::checkCollisions(float dt) {
    // ������������ � ����������
    PaddleCollision::checkPaddleCollision(state.getBall(), state.getPaddle(), hitPaddleSound);

    // ������������ � �������
    BlockCollision::checkBlockCollisions(
        state.getBall(),
        state.getBlocks(),
        state.score,
        dt,
        hitBlockSound,
        hitRockSound,
        state.spatialGrid
    );

    // ������������ �� ������� (�������� ����)
    WallCollision::checkWallCollisions(state.getBall(), window); // ����������

    // �������� ������ ����
    if (state.getBall().getPosition().y + state.getBall().getBounds().height / 2 >= state.getWindowHeight()) {
        state.lives--;
        if (state.lives <= 0) {
            state.gameOver = true;
        }
        else {
            state.waitingLaunch = true;
            state.resetBallPosition();
        }
    }

    // ���������� ����� ����� ���������
    state.spatialGrid.clear();
    for (auto& block : state.getBlocks()) {
        if (!block->isDestroyed()) state.spatialGrid.addBlock(block.get());
    }
}