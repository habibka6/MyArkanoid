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

        // �������� ������ ����������
        void render(float interpolationFactor = 1.0f);
        void clear(const sf::Color& color = sf::Color::Black);
        void display();

        // ��������� ��������� ����� ��������
        void renderBalls(const std::vector<std::unique_ptr<Ball>>& balls, float alpha = 1.0f);
        void renderPaddles(const std::vector<std::unique_ptr<Paddle>>& paddles);
        void renderBlocks(const std::vector<std::unique_ptr<BaseBlock>>& blocks);
        void renderActiveEffects(const std::vector<std::unique_ptr<PowerUpEffect>>& effects);

        // UI � �������
        void renderUI(int score, int lives, int level);
        void renderBackground();
        void renderParticles(); // ��� ������� ��������

        // ��������� ����������
        void setVSync(bool enabled);
        void setInterpolation(bool enabled);
        bool isInterpolationEnabled() const;

        // ��������� �������� ����
        sf::Vector2u getWindowSize() const;

    private:
        sf::RenderWindow& window;
        bool interpolationEnabled;

        // UI ��������
        sf::Font font;
        sf::Text scoreText;
        sf::Text livesText;
        sf::Text levelText;

        // ���
        sf::Sprite backgroundSprite;
        bool hasBackground;

        // ������ �������������
        void initializeUI();
        void loadBackground();

        // ��������������� ������
        void setupText(sf::Text& text, const std::string& content, float x, float y, int size = 24);
    };

} // namespace Arkanoid
