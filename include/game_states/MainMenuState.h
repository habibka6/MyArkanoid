#pragma once
#include "State.h"
#include "InputSystem.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

namespace Arkanoid {

    struct MenuItem {
        sf::Text text;
        std::function<void()> action;
        sf::FloatRect bounds;
        bool highlighted;

        MenuItem(const std::string& label, std::function<void()> callback);
    };

    class MainMenuState : public State {
    private:
        InputSystem inputSystem;
        sf::Font font;
        sf::Text titleText;
        sf::Sprite backgroundSprite;

        std::vector<MenuItem> menuItems;
        int selectedItemIndex;
        bool assetsLoaded;

    public:
        explicit MainMenuState(GameEngine& engine);

        void enter() override;
        void exit() override;
        void update(float deltaTime) override;
        void render(sf::RenderWindow& window) override;
        void handleEvent(const sf::Event& event) override;

    private:
        void loadAssets();
        void initializeUI();
        void initializeMenuItems();
        void initializeInputBindings();

        void navigateUp();
        void navigateDown();
        void selectItem();
        void updateHighlight();
        void handleMouseInput(const sf::Event& event);

        // Действия меню
        void startGame();
        void showLevelSelect();
        void showOptions();
        void exitGame();

        // Утилиты
        void setupText(sf::Text& text, const std::string& content,
            float x, float y, int size = 24);
        void centerText(sf::Text& text, float y);
    };

} 