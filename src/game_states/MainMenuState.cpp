#include "MainMenuState.h"
#include "GameState.h"
#include "GameEngine.h"
#include "AssetManager.h"
#include "SoundManager.h"

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

        // Запускаем фоновую музыку
        SoundManager::getInstance().playMusic("menu_music.ogg", true);
    }

    void MainMenuState::exit() {
        SoundManager::getInstance().stopMusic();
    }

    void MainMenuState::update(float deltaTime) {
        inputSystem.update();

        // Обновляем подсветку текущего элемента
        updateHighlight();
    }

    void MainMenuState::render(sf::RenderWindow& window) {
        // Рендерим фон
        if (assetsLoaded) {
            window.draw(backgroundSprite);
        }

        // Рендерим заголовок
        window.draw(titleText);

        // Рендерим элементы меню
        for (const auto& item : menuItems) {
            window.draw(item.text);
        }
    }

    void MainMenuState::handleEvent(const sf::Event& event) {
        inputSystem.processEvent(event);

        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
            case sf::Keyboard::Up:
                navigateUp();
                break;
            case sf::Keyboard::Down:
                navigateDown();
                break;
            case sf::Keyboard::Enter:
            case sf::Keyboard::Space:
                selectItem();
                break;
            case sf::Keyboard::Escape:
                exitGame();
                break;
            }
        }

        handleMouseInput(event);
    }

    void MainMenuState::loadAssets() {
        try {
            font = AssetManager::getInstance().getFont("retro.ttf");
            sf::Texture& bgTexture = AssetManager::getInstance().getTexture("menu_background.jpg");
            backgroundSprite.setTexture(bgTexture);

            // Масштабируем фон под размер окна
            sf::Vector2u windowSize = engine.getWindow().getSize();
            sf::Vector2u textureSize = bgTexture.getSize();
            backgroundSprite.setScale(
                static_cast<float>(windowSize.x) / textureSize.x,
                static_cast<float>(windowSize.y) / textureSize.y
            );

            assetsLoaded = true;
        }
        catch (const std::exception& e) {
            assetsLoaded = false;
        }
    }

    void MainMenuState::initializeUI() {
        setupText(titleText, "ARKANOID", 0, 150, 72);
        centerText(titleText, 150);
        titleText.setFillColor(sf::Color::Cyan);
        titleText.setOutlineColor(sf::Color::Blue);
        titleText.setOutlineThickness(3);
    }

    void MainMenuState::initializeMenuItems() {
        menuItems.clear();

        menuItems.emplace_back("START GAME", [this]() { startGame(); });
        menuItems.emplace_back("OPTIONS", [this]() { showOptions(); });
        menuItems.emplace_back("EXIT", [this]() { exitGame(); });

        float startY = 350;
        float spacing = 80;

        for (size_t i = 0; i < menuItems.size(); ++i) {
            setupText(menuItems[i].text, menuItems[i].text.getString(), 0, startY + i * spacing, 48);
            centerText(menuItems[i].text, startY + i * spacing);
            menuItems[i].bounds = menuItems[i].text.getGlobalBounds();
        }

        updateHighlight();
    }

    void MainMenuState::navigateUp() {
        SoundManager::getInstance().playSound(SoundType::ButtonClick);
        selectedItemIndex = (selectedItemIndex - 1 + menuItems.size()) % menuItems.size();
    }

    void MainMenuState::navigateDown() {
        SoundManager::getInstance().playSound(SoundType::ButtonClick);
        selectedItemIndex = (selectedItemIndex + 1) % menuItems.size();
    }

    void MainMenuState::selectItem() {
        if (selectedItemIndex >= 0 && selectedItemIndex < menuItems.size()) {
            SoundManager::getInstance().playSound(SoundType::ButtonClick);
            menuItems[selectedItemIndex].action();
        }
    }

    void MainMenuState::updateHighlight() {
        for (size_t i = 0; i < menuItems.size(); ++i) {
            if (i == selectedItemIndex) {
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
        auto playState = std::make_unique<GameState>(engine, 1);
        engine.getStateMachine().changeState(std::move(playState));
    }

    void MainMenuState::showOptions() {
        // TODO: Реализовать состояние опций
        // Пока что просто играем звук
        SoundManager::getInstance().playSound(SoundType::ButtonClick);
    }

    void MainMenuState::exitGame() {
        engine.quit();
    }

    void MainMenuState::setupText(sf::Text& text, const std::string& content,
        float x, float y, int size) {
        text.setFont(font);
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

} // namespace Arkanoid
