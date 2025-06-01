#include "RenderSystem.h"
#include "AssetManager.h"

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
        // Очищаем экран
        clear();

        // Рендерим фон
        renderBackground();

        // Здесь будет рендеринг игровых объектов через вызовы из GameState
    }

    void RenderSystem::clear(const sf::Color& color) {
        window.clear(color);
    }

    void RenderSystem::display() {
        window.display();
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

    sf::Color getEffectColor(const PowerUpEffect& effect) {
        switch (effect.getType()) {
        case PowerUpType::ExtraLife: return sf::Color::Red;
        case PowerUpType::ExpandPaddle: return sf::Color::Green;
        case PowerUpType::ShrinkPaddle: return sf::Color::Yellow;
        case PowerUpType::SlowBall: return sf::Color::Blue;
        default: return sf::Color::White; // Всегда добавляйте default case!
        }
    }
    void RenderSystem::renderActiveEffects(const std::vector<std::unique_ptr<PowerUpEffect>>& effects)
    {
        float x = Config::Window::WIDTH - 50.0f;
        float y = 20.0f;

        for (const auto& effect : effects)
        {
            // Используем оператор * для получения ссылки на объект
            sf::RectangleShape icon(sf::Vector2f(30, 30));
            icon.setPosition(x, y);
            icon.setFillColor(getEffectColor(*effect)); // Здесь разыменовываем unique_ptr
            icon.setOutlineThickness(1);
            icon.setOutlineColor(sf::Color::White);
            window.draw(icon);

            // Дополнительно: добавьте отображение таймера, как в предыдущих примерах
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
        // Обновляем тексты
        scoreText.setString("Score: " + std::to_string(score));
        livesText.setString("Lives: " + std::to_string(lives));
        levelText.setString("Level: " + std::to_string(level));

        // Рендерим UI элементы
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

        }
        catch (const std::exception& e) {
            // Если не удалось загрузить шрифт, используем системный
            if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
                // Создаем fallback UI без шрифта
            }
        }
    }

    void RenderSystem::loadBackground() {
        try {
            sf::Texture& bgTexture = AssetManager::getInstance().getTexture("background.png");
            backgroundSprite.setTexture(bgTexture);

            // Масштабируем фон под размер окна
            sf::Vector2u windowSize = window.getSize();
            sf::Vector2u textureSize = bgTexture.getSize();

            backgroundSprite.setScale(
                static_cast<float>(windowSize.x) / textureSize.x,
                static_cast<float>(windowSize.y) / textureSize.y
            );

            hasBackground = true;

        }
        catch (const std::exception& e) {
            hasBackground = false; // Используем просто цветной фон
        }
    }

    void RenderSystem::setupText(sf::Text& text, const std::string& content, float x, float y, int size) {
        text.setFont(font);
        text.setString(content);
        text.setCharacterSize(size);
        text.setFillColor(sf::Color::White);
        text.setPosition(x, y);

        // Добавляем контур для лучшей читаемости
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1);
    }

} // namespace Arkanoid
