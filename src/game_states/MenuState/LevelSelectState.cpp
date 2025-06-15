#include "LevelSelectState.h"
#include "GameState.h"

namespace Arkanoid {

    LevelSelectState::LevelSelectState(GameEngine& engine)
        : MenuStateBase(engine) {
        setupText(titleText, "SELECT LEVEL", 100, 60);
        setupMenu();
    }

    void LevelSelectState::enter() {
        MenuStateBase::enter();
        updateHighlight();
    }

    void LevelSelectState::setupMenu() {
        levelButtons.clear();

        const int totalLevels = ROWS * COLS; 
        const float buttonW = 120, buttonH = 70;
        const float spacingX = 100, spacingY = 70;

        // Размер сетки
        float gridW = COLS * buttonW + (COLS - 1) * spacingX;
        float gridH = ROWS * buttonH + (ROWS - 1) * spacingY;

        // Центрирование
        float startX = (engine.getWindow().getSize().x - gridW) / 2.0f;
        float startY = (engine.getWindow().getSize().y - gridH) / 2.0f + 40; 

        int level = 1;
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                if (level > totalLevels) break;
                LevelButton btn;
                btn.levelNumber = level;

                float x = startX + col * (buttonW + spacingX);
                float y = startY + row * (buttonH + spacingY);

                btn.rect.setSize(sf::Vector2f(buttonW, buttonH));
                btn.rect.setPosition(x, y);
                btn.rect.setFillColor(sf::Color(70, 70, 140));
                btn.rect.setOutlineColor(sf::Color::Black);
                btn.rect.setOutlineThickness(2);

                setupText(btn.text, std::to_string(level), 0, 32);
   
                auto tBounds = btn.text.getLocalBounds();
                btn.text.setPosition(
                    x + (buttonW - tBounds.width) / 2 - tBounds.left,
                    y + (buttonH - tBounds.height) / 2 - tBounds.top
                );

                levelButtons.push_back(btn);
                ++level;
            }
        }
    }


    void LevelSelectState::render(sf::RenderWindow& window) {
        MenuStateBase::render(window); 
        for (const auto& btn : levelButtons) {
            window.draw(btn.rect);
            window.draw(btn.text);
        }
    }

    void LevelSelectState::handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
            case sf::Keyboard::Up:
                selectedRow = (selectedRow - 1 + ROWS) % ROWS;
                updateHighlight();
                SoundManager::getInstance().playSound(SoundType::ButtonClick);
                break;
            case sf::Keyboard::Down:
                selectedRow = (selectedRow + 1) % ROWS;
                updateHighlight();
                SoundManager::getInstance().playSound(SoundType::ButtonClick);
                break;
            case sf::Keyboard::Left:
                selectedCol = (selectedCol - 1 + COLS) % COLS;
                updateHighlight();
                SoundManager::getInstance().playSound(SoundType::ButtonClick);
                break;
            case sf::Keyboard::Right:
                selectedCol = (selectedCol + 1) % COLS;
                updateHighlight();
                SoundManager::getInstance().playSound(SoundType::ButtonClick);
                break;
            case sf::Keyboard::Enter:
            case sf::Keyboard::Space:
                onSelect(getSelectedIndex());
                break;
            case sf::Keyboard::Escape:
                onBack();
                break;
            default: break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            for (int i = 0; i < levelButtons.size(); ++i) {
                if (levelButtons[i].rect.getGlobalBounds().contains(mousePos)) {
                    selectedRow = i / COLS;
                    selectedCol = i % COLS;
                    updateHighlight();
                    onSelect(i);
                    break;
                }
            }
        }
    }

    void LevelSelectState::updateHighlight() {
        for (int i = 0; i < levelButtons.size(); ++i) {
            bool selected = (i == getSelectedIndex());
            levelButtons[i].rect.setFillColor(selected ? sf::Color(100, 100, 200) : sf::Color(70, 70, 140));
            levelButtons[i].rect.setOutlineColor(selected ? sf::Color::Yellow : sf::Color::Black);
            levelButtons[i].text.setFillColor(selected ? sf::Color::Yellow : sf::Color::White);
        }
    }

    void LevelSelectState::onSelect(int idx) {
        if (idx >= 0 && idx < levelButtons.size()) {
            SoundManager::getInstance().playSound(SoundType::ButtonClick);
            int level = levelButtons[idx].levelNumber;
            engine.getStateMachine().changeState(std::make_unique<GameState>(engine, level));
        }
    }

    void LevelSelectState::onBack() {
        engine.getStateMachine().popState();
    }

}
