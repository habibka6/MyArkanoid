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
    public:
        MainMenuState(GameEngine& engine);
        ~MainMenuState() = default;

        void update(float deltaTime) override;
        void render(sf::RenderWindow& window) override;
        void handleEvent(const sf::Event& event) override;

        void enter() override;
        void exit() override;

    private:
        // UI ��������
        sf::Font font;
        sf::Text titleText;
        sf::Sprite backgroundSprite;
        std::vector<MenuItem> menuItems;

        // ���������� ����
        int selectedItemIndex;
        InputSystem inputSystem;

        // ���������
        bool assetsLoaded;

        // ������ �������������
        void initializeUI();
        void initializeMenuItems();
        void loadAssets();

        // ���������� ����
        void navigateUp();
        void navigateDown();
        void selectItem();
        void updateHighlight();
        void handleMouseInput(const sf::Event& event);

        // ������� ����
        void startGame();
        void showOptions();
        void exitGame();

        // �������
        void setupText(sf::Text& text, const std::string& content,
            float x, float y, int size = 36);
        void centerText(sf::Text& text, float y);
    };

} // namespace Arkanoid
