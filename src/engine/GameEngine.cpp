#include "GameEngine.h"

GameEngine::GameEngine()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Arkanoid"),
    state(WINDOW_WIDTH, WINDOW_HEIGHT),
    renderer(window, state),
    collisionManager(state, window, // Передаем окно в CollisionManager
        soundManager.hitPaddleSound,
        soundManager.hitBlockSound,
        soundManager.hitRockSound
    ) {

    // Настройка окна
    sf::Image icon = AssetManager::getInstance().GetImage("icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setFramerateLimit(70);
}

void GameEngine::run() {
    sf::Clock clock;
    const float fixedStep = 1.f / 120.f;
    float accumulator = 0.f;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        accumulator += dt;

        inputHandler.handleInput(window, state, dt);

        // Фиксированные обновления
        while (accumulator >= fixedStep) {
            update(fixedStep);
            accumulator -= fixedStep;
        }

        // Отрисовка
        renderer.render(accumulator / fixedStep);
    }
}
void GameEngine::update(float dt) {
    if (state.gameOver) return;
    
    // Сначала обновляем позицию платформы
    state.paddle.update(dt, state.getWindowWidth());
    
    if (state.waitingLaunch) {
        // Привязываем мяч к платформе КАЖДЫЙ КАДР
        state.resetBallPosition();
        state.ball.savePrevPosition(); // Сохраняем позицию для интерполяции
    } else {
        // Обновляем мяч и проверяем столкновения
        state.ball.update(dt);
        collisionManager.checkCollisions(dt);
        
        // Проверка потери мяча
        if (state.ball.getPosition().y + state.ball.getBounds().height/2 >= state.getWindowHeight()) {
            soundManager.loseBallSound.play();
            state.lives--;
            if (state.lives <= 0) {
                state.gameOver = true;
            } else {
                state.waitingLaunch = true;
            }
        }
    }
}