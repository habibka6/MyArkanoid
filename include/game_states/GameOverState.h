#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>

namespace Arkanoid {

    enum class GameOverReason { Victory, Defeat };

    class GameOverState : public State {
    public:
        GameOverState(GameEngine& engine, GameOverReason reason, int finalScore, int levelsCompleted);

        void enter() override;
        void render(sf::RenderWindow& window) override;
        void update(float) override {}
        void handleEvent(const sf::Event& event) override;

    private:
        GameOverReason gameResult;
        int finalScore;
        int levelsCompleted;

        sf::Font font;
        sf::Text titleText;
        sf::Text infoText;
        sf::RectangleShape fadeRect;
        float fadeAlpha = 0.f;
        bool canProceed = false;
        sf::Clock timer;
    };

}
