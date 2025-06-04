#include "RenderSystem.h"
#include "AssetManager.h"
#include "Config.h"
#include <iostream>

namespace Arkanoid {

    RenderSystem::RenderSystem(sf::RenderWindow& window)
        : window(window),
        interpolationEnabled(true),
        hasBackground(false) {

        initializeUI();
        loadBackground();
        setVSync(true);
    }

    void RenderSystem::render(float interpolationFactor) {
        // Базовый рендеринг - только фон
        // Используется когда нет специфичного контента
        renderBackground();
    }

    void RenderSystem::clear(const sf::Color& color) {
        window.clear(color);
    }

    void RenderSystem::display() {
        window.display();
    }

    void RenderSystem::renderGameContent(
        const Ball& ball,
        const Paddle& paddle,
        const std::vector<std::unique_ptr<BaseBlock>>& blocks,
        const std::vector<std::unique_ptr<BasePowerUp>>& powerups,
        const std::vector<std::unique_ptr<PowerUpEffect>>& activeEffects,
        int score, int lives, int level) {

        // Рендеринг фона
        renderBackground();

        // Рендеринг игровых объектов
        ball.draw(window);
        paddle.draw(window);

        for (const auto& block : blocks) {
            if (block->isActive()) {
                block->draw(window);
            }
        }

        for (const auto& powerup : powerups) {
            if (powerup->isActive()) {
                powerup->draw(window);
            }
        }

        // Рендеринг активных эффектов
        renderActiveEffects(activeEffects);

        // Рендеринг UI
        renderUI(score, lives, level);
    }

    void RenderSystem::renderBalls(const std::vector<std::unique_ptr<Ball>>& balls, float alpha) {
        for (const auto& ball : balls) {
            if (ball->isActive()) {
                if (interpolationEnabled && alpha < 1.0f) {
                    ball->draw(window, alpha);
                }
                else {
                    ball->draw(window);
                }
            }
        }
    }

    void RenderSystem::renderPaddles(const std::vector<std::unique_ptr<Paddle>>& paddles) {
        for (const auto& paddle : paddles) {
            if (paddle->isActive()) {
                paddle->draw(window);
            }
        }
    }

    void RenderSystem::renderBlocks(const std::vector<std::unique_ptr<BaseBlock>>& blocks) {
        for (const auto& block : blocks) {
            if (block->isActive()) {
                block->draw(window);
            }
        }
    }

    void RenderSystem::renderPowerUps(const std::vector<std::unique_ptr<BasePowerUp>>& powerups) {
        for (const auto& powerup : powerups) {
            if (powerup->isActive()) {
                powerup->draw(window);
            }
        }
    }

    sf::Color getEffectColor(const PowerUpEffect& effect) {
        switch (effect.getType()) {
        case PowerUpType::ExtraLife: return sf::Color::Red;
        case PowerUpType::ExpandPaddle: return sf::Color::Green;
        case PowerUpType::ShrinkPaddle: return sf::Color::Yellow;
        case PowerUpType::SlowBall: return sf::Color::Blue;
        default: return sf::Color::White;
        }
    }

    void RenderSystem::renderActiveEffects(const std::vector<std::unique_ptr<PowerUpEffect>>& effects) {
        float x = Config::Window::WIDTH - 50.0f;
        float y = 20.0f;

        for (const auto& effect : effects) {
            sf::RectangleShape icon(sf::Vector2f(30, 30));
            icon.setPosition(x, y);
            icon.setFillColor(getEffectColor(*effect));
            icon.setOutlineThickness(1);
            icon.setOutlineColor(sf::Color::White);
            window.draw(icon);

            if (effect->isTemporary()) {
                int seconds = static_cast<int>(std::ceil(effect->getRemainingTime()));

                sf::Text timer;
                timer.setFont(font);
                timer.setString(std::to_string(seconds));
                timer.setCharacterSize(14);
                timer.setPosition(x + 35, y + 5);
                window.draw(timer);
            }

            y += 40.0f;
        }
    }

    void RenderSystem::renderUI(int score, int lives, int level) {
        scoreText.setString("Score: " + std::to_string(score));
        livesText.setString("Lives: " + std::to_string(lives));
        levelText.setString("Level: " + std::to_string(level));

        window.draw(scoreText);
        window.draw(livesText);
        window.draw(levelText);
    }

    void RenderSystem::renderBackground() {
        if (hasBackground) {
            window.draw(backgroundSprite);
        }
    }

    void RenderSystem::renderParticles() {
        // Заглушка для будущей системы частиц
    }

    void RenderSystem::renderPauseOverlay() {
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(Config::Window::WIDTH, Config::Window::HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 128));
        window.draw(overlay);

        centerText(pauseText);
        window.draw(pauseText);
    }

    void RenderSystem::renderGameOverOverlay() {
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(Config::Window::WIDTH, Config::Window::HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(overlay);

        centerText(gameOverText);
        window.draw(gameOverText);
    }

    void RenderSystem::renderLevelCompleteOverlay() {
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(Config::Window::WIDTH, Config::Window::HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 100));
        window.draw(overlay);

        centerText(levelCompleteText);
        window.draw(levelCompleteText);
    }

    void RenderSystem::renderDebugInfo(const Ball& ball, const Paddle& paddle, float gameTimer) {
        // Здесь можно добавить отладочную информацию
    }

    void RenderSystem::setVSync(bool enabled) {
        window.setVerticalSyncEnabled(enabled);
    }

    void RenderSystem::setInterpolation(bool enabled) {
        interpolationEnabled = enabled;
    }

    bool RenderSystem::isInterpolationEnabled() const {
        return interpolationEnabled;
    }

    sf::Vector2u RenderSystem::getWindowSize() const {
        return window.getSize();
    }

    void RenderSystem::initializeUI() {
        try {
            font = AssetManager::getInstance().getFont("retro.ttf");

            setupText(scoreText, "Score: 0", 10, 10);
            setupText(livesText, "Lives: 3", 10, 40);
            setupText(levelText, "Level: 1", 10, 70);

            // Инициализация текстов состояний
            pauseText.setFont(font);
            pauseText.setString("PAUSED");
            pauseText.setCharacterSize(72);
            pauseText.setFillColor(sf::Color::Yellow);

            gameOverText.setFont(font);
            gameOverText.setString("GAME OVER");
            gameOverText.setCharacterSize(72);
            gameOverText.setFillColor(sf::Color::Red);

            levelCompleteText.setFont(font);
            levelCompleteText.setString("LEVEL COMPLETE!");
            levelCompleteText.setCharacterSize(48);
            levelCompleteText.setFillColor(sf::Color::Green);

        }
        catch (const std::exception& e) {
            if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
                // Создаем fallback UI без шрифта
            }
        }
    }

    void RenderSystem::loadBackground() {
        try {
            sf::Texture& bgTexture = AssetManager::getInstance().getTexture("background.png");
            backgroundSprite.setTexture(bgTexture);

            sf::Vector2u windowSize = window.getSize();
            sf::Vector2u textureSize = bgTexture.getSize();

            backgroundSprite.setScale(
                static_cast<float>(windowSize.x) / textureSize.x,
                static_cast<float>(windowSize.y) / textureSize.y
            );

            hasBackground = true;

        }
        catch (const std::exception& e) {
            hasBackground = false;
        }
    }

    void RenderSystem::setupText(sf::Text& text, const std::string& content, float x, float y, int size) {
        text.setFont(font);
        text.setString(content);
        text.setCharacterSize(size);
        text.setFillColor(sf::Color::White);
        text.setPosition(x, y);

        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1);
    }

    void RenderSystem::centerText(sf::Text& text) {
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            (Config::Window::WIDTH - textBounds.width) / 2,
            (Config::Window::HEIGHT - textBounds.height) / 2
        );
    }

} // namespace Arkanoid
