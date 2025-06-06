#pragma once
#include "State.h"
#include "InputSystem.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

namespace Arkanoid {

    struct OptionItem {
        sf::Text labelText;
        sf::Text valueText;
        std::function<void(int)> onChange; 
        std::vector<std::string> values;   
        int currentIndex;
        sf::FloatRect bounds;

        OptionItem(const std::string& label, const std::vector<std::string>& vals,
            int startIndex, std::function<void(int)> callback);
    };

    class OptionsState : public State {
    private:
        InputSystem inputSystem;
        sf::Font font;
        sf::Text titleText;
        sf::Text instructionText;
        sf::Sprite backgroundSprite;

        std::vector<OptionItem> options;
        int selectedOptionIndex;
        bool assetsLoaded;

    public:
        OptionsState(GameEngine& engine);

        void enter() override;
        void exit() override;
        void update(float deltaTime) override;
        void render(sf::RenderWindow& window) override;
        void handleEvent(const sf::Event& event) override;

    private:
        void loadAssets();
        void initializeUI();
        void initializeOptions();
        void initializeInputBindings();
        void navigateUp();
        void navigateDown();
        void changeValue(int direction); 
        void updateHighlight();
        void returnToMenu();

        void setupText(sf::Text& text, const std::string& content,
            float x, float y, int size = 24);
        void centerText(sf::Text& text, float y);
    };

}