#include "GameOverState.h"
#include "MainMenuState.h"
#include "GameState.h"
#include "AssetManager.h"
#include "SoundManager.h"

namespace Arkanoid {

    GameOverState::GameOverState(GameEngine& engine, GameOverReason reason, int finalScore, int levelsCompleted)
        : State(engine), gameResult(reason), finalScore(finalScore), levelsCompleted(levelsCompleted)
    {
        font = AssetManager::getInstance().getFont("retro.ttf");

        // Заголовок
        std::string title = (gameResult == GameOverReason::Victory) ? "VICTORY!" : "GAME OVER";
        titleText.setFont(font);
        titleText.setString(title);
        titleText.setCharacterSize(80);
        titleText.setFillColor(gameResult == GameOverReason::Victory ? sf::Color::Green : sf::Color::Red);
        auto bounds = titleText.getLocalBounds();
        titleText.setPosition((engine.getWindow().getSize().x - bounds.width) / 2, 180 );

        // Информация о счёте
        std::string info = "Final Score: " + std::to_string(finalScore) +
            "\nLevels Completed: " + std::to_string(levelsCompleted) +
            "\n\nPress ENTER or ESC to return to menu";
        infoText.setFont(font);
        infoText.setString(info);
        infoText.setCharacterSize(32);
        infoText.setFillColor(sf::Color::White);
        auto ibounds = infoText.getLocalBounds();
        infoText.setPosition((engine.getWindow().getSize().x - ibounds.width) / 2, 320);

        fadeRect.setSize(sf::Vector2f(engine.getWindow().getSize()));
        fadeRect.setFillColor(sf::Color(0, 0, 0, 180));
    }

    void GameOverState::enter() {
        try {
            std::string endgame = (gameResult == GameOverReason::Victory) ? "victory_music.ogg" : "game_over.wav";
            SoundManager::getInstance().playSound(endgame);
        }
        catch (...) {}
        fadeAlpha = 0.f;
        canProceed = false;
        timer.restart();
    }

    void GameOverState::render(sf::RenderWindow& window) {
        fadeAlpha = std::min(180.f, fadeAlpha + 2.5f); 
        fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(fadeAlpha)));
        window.draw(fadeRect);

        window.draw(titleText);
        window.draw(infoText);
    }

    void GameOverState::handleEvent(const sf::Event& event) {
        if (timer.getElapsedTime().asSeconds() < 1.0f)
            return;

        canProceed = true;

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Escape) {
                engine.getStateMachine().changeState(std::make_unique<MainMenuState>(engine));
            }
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            engine.getStateMachine().changeState(std::make_unique<MainMenuState>(engine));
        }
    }

}
