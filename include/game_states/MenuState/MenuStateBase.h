#pragma once
#include "State.h"
#include "GameEngine.h"
#include <vector>
#include <functional>
#include <SFML/Graphics.hpp>

namespace Arkanoid {

    struct MenuItem {
        std::string label;
        std::function<void()> action;
        sf::Text text;
    };

    class MenuStateBase : public State {
    public:
        MenuStateBase(GameEngine& engine);
        virtual ~MenuStateBase() = default;

        void enter() override;
        void update(float) override {}
        void render(sf::RenderWindow& window) override;
        void handleEvent(const sf::Event& event) override;

    protected:
        std::vector<MenuItem> menuItems;
        int selectedItem = 0;
        sf::Font font;
        sf::Text titleText;
        sf::Sprite backgroundSprite;

        virtual void onSelect(int idx);
        virtual void onBack() {}
        virtual void setupMenu() = 0; // реализуется в наследниках

        void navigate(int dir);
        void updateHighlight();
        void setupText(sf::Text& text, const std::string& str, float y, int size = 48);
        void centerText(sf::Text& text, float y);
    };

}
