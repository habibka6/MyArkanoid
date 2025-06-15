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
        const std::vector<std::unique_ptr<PowerUp>>& powerups,
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

        // Рендеринг UI
        renderUI(score, lives, level);
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


    void RenderSystem::renderPauseOverlay() {
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(Config::Window::WIDTH, Config::Window::HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 128));
        window.draw(overlay);

        centerText(pauseText);
        window.draw(pauseText);
    }

    void RenderSystem::renderLevelCompleteOverlay() {
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(Config::Window::WIDTH, Config::Window::HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 100));
        window.draw(overlay);

        centerText(levelCompleteText);
        window.draw(levelCompleteText);
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


            levelCompleteText.setFont(font);
            levelCompleteText.setString("LEVEL COMPLETE!");
            levelCompleteText.setCharacterSize(48);
            levelCompleteText.setFillColor(sf::Color::Green);

        }
        catch (const std::exception& e) {
            if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
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

} 
