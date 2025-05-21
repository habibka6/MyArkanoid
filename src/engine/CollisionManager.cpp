#include "CollisionManager.h"

CollisionManager::CollisionManager(GameState& state, const sf::RenderWindow& window,
    sf::Sound& paddleSound, sf::Sound& blockSound, sf::Sound& rockSound)
    : state(state),
    window(window), // Инициализация окна
    hitPaddleSound(paddleSound),
    hitBlockSound(blockSound),
    hitRockSound(rockSound) {
}

void CollisionManager::checkCollisions(float dt) {
    // Столкновение с платформой
    PaddleCollision::checkPaddleCollision(state.ball, state.paddle, hitPaddleSound);

    // Столкновение с блоками
    BlockCollision::checkBlockCollisions(
        state.ball,
        state.blocks,
        state.score,
        dt,
        hitBlockSound,
        hitRockSound,
        state.spatialGrid
    );

    // Столкновение со стенами (передаем окно)
    WallCollision::checkWallCollisions(state.ball, window); // Исправлено

    // Проверка потери мяча
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

    // Обновление сетки после изменений
    state.spatialGrid.clear();
    for (auto& block : state.blocks) {
        if (!block->isDestroyed()) state.spatialGrid.addBlock(block.get());
    }
}