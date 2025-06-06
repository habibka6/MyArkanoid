#include "MainMenuState.h"
#include "GameState.h"
#include "OptionsState.h"
#include "LevelSelectState.h"
#include "GameEngine.h"
#include "AssetManager.h"
#include "SoundManager.h"
#include "Config.h"

namespace Arkanoid {

    MenuItem::MenuItem(const std::string& label, std::function<void()> callback)
        : action(callback), highlighted(false) {
        text.setString(label);
    }

    MainMenuState::MainMenuState(GameEngine& engine)
        : State(engine), selectedItemIndex(0), assetsLoaded(false) {
    }

    void MainMenuState::enter() {
        loadAssets();
        initializeUI();
        initializeMenuItems();
        initializeInputBindings();


        try {
            SoundManager::getInstance().playMusic("menu_music.ogg", true);
        }
        catch (...) { }
    }

    void MainMenuState::exit() {
        SoundManager::getInstance().stopMusic();
    }

    void MainMenuState::update(float deltaTime) {
        inputSystem.update();
        updateHighlight();
    }

    void MainMenuState::render(sf::RenderWindow& window) {

        if (assetsLoaded && backgroundSprite.getTexture()) {
            window.draw(backgroundSprite);
        }


        window.draw(titleText);

        for (const auto& item : menuItems) {
            window.draw(item.text);
        }
    }


    void MainMenuState::initializeInputBindings() {
        // Навигация
        inputSystem.bindKeyPress(sf::Keyboard::Up,
            std::make_unique<LambdaCommand>([this]() {
                navigateUp();
                }));

        inputSystem.bindKeyPress(sf::Keyboard::Down,
            std::make_unique<LambdaCommand>([this]() {
                navigateDown();
                }));

        // Выбор пункта
        inputSystem.bindKeyPress(sf::Keyboard::Enter,
            std::make_unique<LambdaCommand>([this]() {
                selectItem();
                }));

        inputSystem.bindKeyPress(sf::Keyboard::Space,
            std::make_unique<LambdaCommand>([this]() {
                selectItem();
                }));

        // Выход
        inputSystem.bindKeyPress(sf::Keyboard::Escape,
            std::make_unique<LambdaCommand>([this]() {
                exitGame();
                }));
    }



    void MainMenuState::handleEvent(const sf::Event& event) {
        inputSystem.processEvent(event);
        handleMouseInput(event);
    }

    void MainMenuState::loadAssets() {
        try {
            try {
                font = AssetManager::getInstance().getFont("retro.ttf");
            }
            catch (...) {
                if (!font.loadFromFile("assets/fonts/arial.ttf")) {}
            }

   
            try {
                sf::Texture& bgTexture = AssetManager::getInstance().getTexture("menu_background.jpg");
                backgroundSprite.setTexture(bgTexture);

          
                sf::Vector2u windowSize = engine.getWindow().getSize();
                sf::Vector2u textureSize = bgTexture.getSize();

                if (textureSize.x > 0 && textureSize.y > 0) {
                    backgroundSprite.setScale(
                        static_cast<float>(windowSize.x) / textureSize.x,
                        static_cast<float>(windowSize.y) / textureSize.y
                    );
                }
            }
            catch (...) { }

            assetsLoaded = true;
        }
        catch (const std::exception& e) {
            assetsLoaded = false;
        }
    }

    void MainMenuState::initializeUI() {
        // Настройка заголовка
        setupText(titleText, "ARKANOID", 0, 150, 72);
        centerText(titleText, 150);
        titleText.setFillColor(sf::Color::Cyan);
        titleText.setOutlineColor(sf::Color::Blue);
        titleText.setOutlineThickness(3);
    }

    void MainMenuState::initializeMenuItems() {
        menuItems.clear();

        // Создание элементов меню
        menuItems.emplace_back("START GAME", [this]() { startGame(); });
        menuItems.emplace_back("SELECT LEVEL", [this]() { showLevelSelect(); });
        menuItems.emplace_back("OPTIONS", [this]() { showOptions(); });
        menuItems.emplace_back("EXIT", [this]() { exitGame(); });

        // Позиционирование элементов
        float startY = 350;
        float spacing = 80;

        for (size_t i = 0; i < menuItems.size(); ++i) {
            float y = startY + i * spacing;
            setupText(menuItems[i].text, menuItems[i].text.getString(), 0, y, 48);
            centerText(menuItems[i].text, y);
            menuItems[i].bounds = menuItems[i].text.getGlobalBounds();
        }

        updateHighlight();
    }

    void MainMenuState::navigateUp() {
        try {
            SoundManager::getInstance().playSound(SoundType::ButtonClick);
        }
        catch (...) { }

        if (!menuItems.empty()) {
            selectedItemIndex = (selectedItemIndex - 1 + menuItems.size()) % menuItems.size();
        }
    }

    void MainMenuState::navigateDown() {
        try {
            SoundManager::getInstance().playSound(SoundType::ButtonClick);
        }
        catch (...) { }

        if (!menuItems.empty()) {
            selectedItemIndex = (selectedItemIndex + 1) % menuItems.size();
        }
    }

    void MainMenuState::selectItem() {
        if (selectedItemIndex >= 0 && selectedItemIndex < static_cast<int>(menuItems.size())) {
            try {
                SoundManager::getInstance().playSound(SoundType::ButtonClick);
            }
            catch (...) {
                // Звук не критичен
            }

            if (menuItems[selectedItemIndex].action) {
                menuItems[selectedItemIndex].action();
            }
        }
    }

    void MainMenuState::updateHighlight() {
        for (size_t i = 0; i < menuItems.size(); ++i) {
            if (static_cast<int>(i) == selectedItemIndex) {
                menuItems[i].text.setFillColor(sf::Color::Yellow);
                menuItems[i].text.setOutlineColor(sf::Color::Red);
                menuItems[i].text.setOutlineThickness(2);
                menuItems[i].highlighted = true;
            }
            else {
                menuItems[i].text.setFillColor(sf::Color::White);
                menuItems[i].text.setOutlineColor(sf::Color::Black);
                menuItems[i].text.setOutlineThickness(1);
                menuItems[i].highlighted = false;
            }
        }
    }

    void MainMenuState::handleMouseInput(const sf::Event& event) {
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x),
                static_cast<float>(event.mouseMove.y));

            for (size_t i = 0; i < menuItems.size(); ++i) {
                if (menuItems[i].bounds.contains(mousePos)) {
                    selectedItemIndex = static_cast<int>(i);
                    break;
                }
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                selectItem();
            }
        }
    }

    void MainMenuState::startGame() {
        try {
            auto playState = std::make_unique<GameState>(engine, 1);
            engine.getStateMachine().changeState(std::move(playState));
        }
        catch (const std::exception& e) { }
    }

    void MainMenuState::showLevelSelect() {
        try {
            auto levelSelectState = std::make_unique<LevelSelectState>(engine);
            engine.getStateMachine().pushState(std::move(levelSelectState));
        }
        catch (const std::exception& e) { }
    }

    void MainMenuState::showOptions() {
        try {
            auto optionsState = std::make_unique<OptionsState>(engine);
            engine.getStateMachine().pushState(std::move(optionsState));
        }
        catch (const std::exception& e) {
            try {
                SoundManager::getInstance().playSound(SoundType::Error);
            }
            catch (...) {}
        }
    }

    void MainMenuState::exitGame() {
        engine.quit();
    }

    void MainMenuState::setupText(sf::Text& text, const std::string& content,
        float x, float y, int size) {
        if (font.getInfo().family.empty()) {static sf::Font defaultFont;text.setFont(defaultFont);}
        else {
            text.setFont(font);
        }

        text.setString(content);
        text.setCharacterSize(size);
        text.setFillColor(sf::Color::White);
        text.setPosition(x, y);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1);
    }

    void MainMenuState::centerText(sf::Text& text, float y) {
        sf::FloatRect bounds = text.getLocalBounds();
        float windowWidth = static_cast<float>(engine.getWindow().getSize().x);
        text.setPosition((windowWidth - bounds.width) / 2, y);
    }

} 
