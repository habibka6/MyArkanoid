#pragma once
#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "Paddle.h"
#include "BaseBlock.h"
#include <vector>
#include <memory>
#include"PowerUpEffect.h"
namespace Arkanoid {

    class RenderSystem {
    public:
        RenderSystem(sf::RenderWindow& window);
        ~RenderSystem() = default;

        // Основные методы рендеринга
        void render(float interpolationFactor = 1.0f);
        void clear(const sf::Color& color = sf::Color::Black);
        void display();

        // Рендеринг отдельных типов объектов
        void renderBalls(const std::vector<std::unique_ptr<Ball>>& balls, float alpha = 1.0f);
        void renderPaddles(const std::vector<std::unique_ptr<Paddle>>& paddles);
        void renderBlocks(const std::vector<std::unique_ptr<BaseBlock>>& blocks);
        void renderActiveEffects(const std::vector<std::unique_ptr<PowerUpEffect>>& effects);

        // UI и эффекты
        void renderUI(int score, int lives, int level);
        void renderBackground();
        void renderParticles(); // Для будущих эффектов

        // Настройки рендеринга
        void setVSync(bool enabled);
        void setInterpolation(bool enabled);
        bool isInterpolationEnabled() const;

        // Получение размеров окна
        sf::Vector2u getWindowSize() const;

    private:
        sf::RenderWindow& window;
        bool interpolationEnabled;

        // UI элементы
        sf::Font font;
        sf::Text scoreText;
        sf::Text livesText;
        sf::Text levelText;

        // Фон
        sf::Sprite backgroundSprite;
        bool hasBackground;

        // Методы инициализации
        void initializeUI();
        void loadBackground();

        // Вспомогательные методы
        void setupText(sf::Text& text, const std::string& content, float x, float y, int size = 24);
    };

} // namespace Arkanoid
