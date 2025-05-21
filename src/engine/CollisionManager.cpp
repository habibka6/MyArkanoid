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
    PaddleCollision::checkPaddleCollision(state.ball, state.paddle, hitPaddleSound);

    // ������������ � �������
    BlockCollision::checkBlockCollisions(
        state.ball,
        state.blocks,
        state.score,
        dt,
        hitBlockSound,
        hitRockSound,
        state.spatialGrid
    );

    // ������������ �� ������� (�������� ����)
    WallCollision::checkWallCollisions(state.ball, window); // ����������

    // �������� ������ ����
    if (state.ball.getPosition().y + state.ball.getBounds().height / 2 >= state.getWindowHeight()) {
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
    for (auto& block : state.blocks) {
        if (!block->isDestroyed()) state.spatialGrid.addBlock(block.get());
    }
}