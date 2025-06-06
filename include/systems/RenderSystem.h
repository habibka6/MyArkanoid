#pragma once
#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "Paddle.h"
#include "BaseBlock.h"
#include "BasePowerUp.h"
#include "PowerUpEffect.h"
#include <vector>
#include <memory>

namespace Arkanoid {

    enum class GameStatus {
        Playing,
        Paused,
        LevelComplete,
        GameOver,
        Victory
    };

    class RenderSystem {
    public:
        RenderSystem(sf::RenderWindow& window);
        ~RenderSystem() = default;

        // Основные методы рендеринга
        void render(float interpolationFactor = 1.0f);
        void clear(const sf::Color& color = sf::Color::Black);
        void display();

        // Рендеринг игровых объектов
        void renderBalls(const std::vector<std::unique_ptr<Ball>>& balls, float alpha = 1.0f);
        void renderPaddles(const std::vector<std::unique_ptr<Paddle>>& paddles);
        void renderBlocks(const std::vector<std::unique_ptr<BaseBlock>>& blocks);
        void renderPowerUps(const std::vector<std::unique_ptr<BasePowerUp>>& powerups);
        void renderActiveEffects(const std::vector<std::unique_ptr<PowerUpEffect>>& effects);

        // Рендеринг UI
        void renderUI(int score, int lives, int level);
        void renderBackground();
        void renderParticles();

        // Рендеринг игрового контента
        void renderGameContent(
            const Ball& ball,
            const Paddle& paddle,
            const std::vector<std::unique_ptr<BaseBlock>>& blocks,
            const std::vector<std::unique_ptr<BasePowerUp>>& powerups,
            const std::vector<std::unique_ptr<PowerUpEffect>>& activeEffects,
            int score, int lives, int level
        );

        // Рендеринг оверлеев состояний
        void renderPauseOverlay();
        void renderGameOverOverlay();
        void renderLevelCompleteOverlay();
        void renderDebugInfo(const Ball& ball, const Paddle& paddle, float gameTimer);

        // Настройки
        void setVSync(bool enabled);
        void setInterpolation(bool enabled);
        bool isInterpolationEnabled() const;

        sf::Vector2u getWindowSize() const;

    private:
        sf::RenderWindow& window;
        bool interpolationEnabled;

        // UI элементы
        sf::Font font;
        sf::Text scoreText;
        sf::Text livesText;
        sf::Text levelText;

        // Тексты состояний
        sf::Text pauseText;
        sf::Text gameOverText;
        sf::Text levelCompleteText;

        // Фон
        sf::Sprite backgroundSprite;
        bool hasBackground;

        // Приватные методы инициализации
        void initializeUI();
        void loadBackground();
        void setupText(sf::Text& text, const std::string& content, float x, float y, int size = 24);
        void centerText(sf::Text& text);
    };

} 
