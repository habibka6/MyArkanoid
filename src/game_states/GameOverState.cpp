#include "GameOverState.h"
#include "MainMenuState.h"
#include "GameState.h"
#include "GameEngine.h"
#include "AssetManager.h"
#include "SoundManager.h"
#include "Config.h"
#include <algorithm>

namespace Arkanoid {

    GameOverState::GameOverState(GameEngine& engine, GameOverReason reason,
        int finalScore, int levelsCompleted)
        : State(engine),
        gameResult(reason),
        finalScore(finalScore),
        levelsCompleted(levelsCompleted),
        stateTimer(0.0f),
        overlayAlpha(0.0f) {
    }

    void GameOverState::enter() {
        loadAssets();
        initializeUI();
        initializeInputBindings(); 

        try {
            if (gameResult == GameOverReason::Victory) {
                SoundManager::getInstance().playMusic("victory_music.ogg", false);
            }
            else {
                SoundManager::getInstance().playMusic("defeat_music.ogg", false);
            }
        }
        catch (...) {
           
        }
    }

    void GameOverState::exit() {
        try {
            SoundManager::getInstance().stopMusic();
        }
        catch (...) {
        }
    }

    void GameOverState::update(float deltaTime) {
        inputSystem.update();
        stateTimer += deltaTime;

        if (overlayAlpha < 1.0f) {
            overlayAlpha += deltaTime * 2.0f; 
            overlayAlpha = std::min(overlayAlpha, 1.0f);

            overlayColor.a = static_cast<sf::Uint8>(overlayAlpha * 180);
        }
    }

    void GameOverState::render(sf::RenderWindow& window) {
        if (backgroundSprite.getTexture()) {
            window.draw(backgroundSprite);
        }

        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(static_cast<float>(Config::Window::WIDTH), static_cast<float>(Config::Window::HEIGHT)));
        overlay.setFillColor(overlayColor);
        window.draw(overlay);

        
        window.draw(titleText);
        window.draw(scoreText);
        window.draw(levelText);

       
        if (stateTimer > 2.0f) {
            window.draw(instructionText);
        }
    }

    void GameOverState::handleEvent(const sf::Event& event) {  
        
        inputSystem.processEvent(event);

        
        if (stateTimer < 1.0f) return; 

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                restartGame();
            }
            else if (event.mouseButton.button == sf::Mouse::Right) {
                returnToMenu();
            }
        }
    }

    void GameOverState::loadAssets() {
        try {
            font = AssetManager::getInstance().getFont("retro.ttf");

            try {
                sf::Texture& bgTexture = AssetManager::getInstance().getTexture("background.png");
                backgroundSprite.setTexture(bgTexture);

                sf::Vector2u windowSize = engine.getWindow().getSize();
                sf::Vector2u textureSize = bgTexture.getSize();
                if (textureSize.x > 0 && textureSize.y > 0) {
                    backgroundSprite.setScale(static_cast<float>(windowSize.x) / textureSize.x, static_cast<float>(windowSize.y) / textureSize.y );
                }
            }
            catch (...) {
                
            }
        }
        catch (...) {
            
        }

      
        overlayColor = sf::Color(0, 0, 0, 0);
    }

    void GameOverState::initializeUI() {
        setupTexts();
    }

    void GameOverState::initializeInputBindings() {
        // Перезапуск игры
        inputSystem.bindKeyPress(sf::Keyboard::Enter,
            std::make_unique<LambdaCommand>([this]() {
                if (stateTimer > 1.0f) {
                    restartGame();
                }
                }));

        inputSystem.bindKeyPress(sf::Keyboard::Space,
            std::make_unique<LambdaCommand>([this]() {
                if (stateTimer > 1.0f) {
                    restartGame();
                }
                }));

        // Возврат в меню
        inputSystem.bindKeyPress(sf::Keyboard::Escape,
            std::make_unique<LambdaCommand>([this]() {
                if (stateTimer > 1.0f) {
                    returnToMenu();
                }
                }));

        inputSystem.bindKeyPress(sf::Keyboard::M,
            std::make_unique<LambdaCommand>([this]() {
                if (stateTimer > 1.0f) {
                    returnToMenu();
                }
                }));
    }

    void GameOverState::setupTexts() {

        setupText(titleText, getResultMessage(), 0, 200, 64);
        centerText(titleText, 200);
        titleText.setFillColor(getResultColor());
        titleText.setOutlineColor(sf::Color::Black);
        titleText.setOutlineThickness(3);

     
        setupText(scoreText, "Final Score: " + std::to_string(finalScore), 0, 300, 36);
        centerText(scoreText, 300);

       
        setupText(levelText, "Levels Completed: " + std::to_string(levelsCompleted), 0, 350, 36);
        centerText(levelText, 350);

        
        std::string instructions;
        if (gameResult == GameOverReason::Victory) {
            instructions = "ENTER - Play Again    ESC - Main Menu    LMB - Restart    RMB - Menu";
        }
        else {
            instructions = "ENTER - Retry    ESC - Main Menu    LMB - Restart    RMB - Menu";
        }

        setupText(instructionText, instructions, 0, 500, 24);
        centerText(instructionText, 500);
        instructionText.setFillColor(sf::Color::Yellow);
    }

    void GameOverState::returnToMenu() {
        try {
            SoundManager::getInstance().playSound(SoundType::ButtonClick);
        }
        catch (...) {}

        auto mainMenu = std::make_unique<MainMenuState>(engine);
        engine.getStateMachine().changeState(std::move(mainMenu));
    }

    void GameOverState::restartGame() {
        try {
            SoundManager::getInstance().playSound(SoundType::ButtonClick);
        }
        catch (...) {}

        auto playState = std::make_unique<GameState>(engine, 1);
        engine.getStateMachine().changeState(std::move(playState));
    }

    void GameOverState::setupText(sf::Text& text, const std::string& content,
        float x, float y, int size) {
        if (!font.getInfo().family.empty()) {
            text.setFont(font);
        }
        text.setString(content);
        text.setCharacterSize(size);
        text.setFillColor(sf::Color::White);
        text.setPosition(x, y);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1);
    }

    void GameOverState::centerText(sf::Text& text, float y) {
        sf::FloatRect bounds = text.getLocalBounds();
        float windowWidth = static_cast<float>(engine.getWindow().getSize().x);
        text.setPosition((windowWidth - bounds.width) / 2, y);
    }

    std::string GameOverState::getResultMessage() const {
        switch (gameResult) {
        case GameOverReason::Victory:
            return "VICTORY!";
        case GameOverReason::Defeat:
            return "GAME OVER";
        default:
            return "UNKNOWN";
        }
    }

    sf::Color GameOverState::getResultColor() const {
        switch (gameResult) {
        case GameOverReason::Victory:
            return sf::Color::Green;
        case GameOverReason::Defeat:
            return sf::Color::Red;
        default:
            return sf::Color::White;
        }
    }

} 
