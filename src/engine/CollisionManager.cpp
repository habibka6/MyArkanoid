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
    PaddleCollision::checkPaddleCollision(state.getBall(), state.getPaddle(), hitPaddleSound);

    // Столкновение с блоками
    BlockCollision::checkBlockCollisions(
        state.getBall(),
        state.getBlocks(),
        state.score,
        dt,
        hitBlockSound,
        hitRockSound,
        state.spatialGrid
    );

    // Столкновение со стенами (передаем окно)
    WallCollision::checkWallCollisions(state.getBall(), window); // Исправлено

    // Проверка потери мяча
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

    // Обновление сетки после изменений
    state.spatialGrid.clear();
    for (auto& block : state.getBlocks()) {
        if (!block->isDestroyed()) state.spatialGrid.addBlock(block.get());
    }
}