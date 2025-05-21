#include "Renderer.h"

Renderer::Renderer(sf::RenderWindow& window, const GameState& state)
    : window(window), state(state) {
    initUI();
}

void Renderer::initUI() {
    // Фон
    backgroundSprite.setTexture(AssetManager::getInstance().GetTexture("background.png"));
    backgroundSprite.setScale(
        state.getWindowWidth() / backgroundSprite.getLocalBounds().width,
        state.getWindowHeight() / backgroundSprite.getLocalBounds().height
    );

    // Текст
    scoreText.setFont(AssetManager::getInstance().GetFont("retro.ttf"));
    scoreText.setCharacterSize(30);
    scoreText.setPosition(20, 20);

    gameOverText.setFont(AssetManager::getInstance().GetFont("retro.ttf"));
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(100);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(
        state.getWindowWidth() / 2 - gameOverText.getLocalBounds().width / 2,
        state.getWindowHeight() / 2 - gameOverText.getLocalBounds().height / 2
    );
}

void Renderer::render(float alpha) {
    window.clear();
    window.draw(backgroundSprite);

    if (state.gameOver) {
        window.draw(gameOverText);
    }
    else {
        // Отрисовка платформы и мяча через геттеры
        state.getPaddle().draw(window);
        state.getBall().draw(window, alpha);

        // Отрисовка блоков
        for (const auto& block : state.getBlocks()) {
            if (!block->isDestroyed())
                block->draw(window);
        }

        // Обновление текста счета
        std::stringstream ss;
        ss << "Score: " << state.score << " | Lives: " << state.lives;
        scoreText.setString(ss.str());
        window.draw(scoreText);
    }

    window.display();
}