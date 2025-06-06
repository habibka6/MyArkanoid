#pragma once
#include "State.h"
#include "InputSystem.h"
#include <SFML/Graphics.hpp>

namespace Arkanoid {

    enum class GameOverReason {
        Victory,
        Defeat
    };

    class GameOverState : public State {
    public:
        GameOverState(GameEngine& engine, GameOverReason reason,
            int finalScore, int levelsCompleted);
        ~GameOverState() = default;

        void update(float deltaTime) override;
        void render(sf::RenderWindow& window) override;
        void handleEvent(const sf::Event& event) override;

        void enter() override;
        void exit() override;

    private:
        GameOverReason gameResult;
        int finalScore;
        int levelsCompleted;


        // UI элементы
        sf::Font font;
        sf::Text titleText;
        sf::Text scoreText;
        sf::Text levelText;
        sf::Text instructionText;
        sf::Sprite backgroundSprite;

        // ”правление
        InputSystem inputSystem;
        float stateTimer;

        // Ёффекты
        sf::Color overlayColor;
        float overlayAlpha;

        // ћетоды инициализации
        void initializeUI();
        void loadAssets();
        void initializeInputBindings();
        void setupTexts();

      
        void returnToMenu();
        void restartGame();

     
        void setupText(sf::Text& text, const std::string& content,
            float x, float y, int size = 36);
        void centerText(sf::Text& text, float y);
        std::string getResultMessage() const;
        sf::Color getResultColor() const;
    };

} 
