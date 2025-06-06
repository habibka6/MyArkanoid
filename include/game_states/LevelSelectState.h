#pragma once
#include "State.h"
#include "InputSystem.h"
#include <SFML/Graphics.hpp>
#include <vector>

namespace Arkanoid {

    class GameEngine;

    struct LevelButton {
        int levelNumber;
        sf::RectangleShape background;
        sf::RectangleShape border;
        sf::Text numberText;
        sf::FloatRect bounds;
        bool selected;

        LevelButton(int number) : levelNumber(number), selected(false) {}
    };

    class LevelSelectState : public State {
    private:
        static const int LEVELS_PER_ROW = 5;
        static const int LEVELS_PER_COLUMN = 4;
        static const int LEVELS_PER_PAGE = LEVELS_PER_ROW * LEVELS_PER_COLUMN;
        static const int MAX_LEVELS = 50;

        // UI элементы
        sf::Font font;
        sf::Text titleText;
        sf::Text pageText;
        sf::Text instructionText;
        sf::Sprite backgroundSprite;

        // Уровни
        std::vector<LevelButton> levelButtons;
        int selectedLevelIndex;
        int currentPage;
        int totalPages;

        // Системы
        InputSystem inputSystem;

    public:
        LevelSelectState(GameEngine& engine);

        void enter() override;
        void exit() override;
        void update(float deltaTime) override;
        void render(sf::RenderWindow& window) override;
        void handleEvent(const sf::Event& event) override;

    private:
        // Инициализация
        void loadAssets();
        void initializeUI();
        void setupCurrentPage();
        void updatePageDisplay();
        void initializeInputBindings();
        // Навигация
        void navigateUp();
        void navigateDown();
        void navigateLeft();
        void navigateRight();
        void previousPage();
        void nextPage();
        void selectLevel();
        void returnToMenu();

        // Обновления
        void updateSelection();

        // Утилиты
        void setupText(sf::Text& text, const std::string& content, float x, float y, int size);
        void centerTextHorizontally(sf::Text& text, float y);
        int getSelectedRow() const;
        int getSelectedColumn() const;
        void setSelectedPosition(int row, int col);
        bool isValidLevelIndex(int index) const;
        sf::Color getButtonColor(bool selected) const;
    };

} 
