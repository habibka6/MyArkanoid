#include "GameEngine.h"
#include "MainMenuState.h"
#include <iostream>

namespace Arkanoid {

    const float GameEngine::TARGET_FPS = Config::Game::TARGET_FPS;
    const float GameEngine::TIME_STEP = 1.0f / TARGET_FPS;

    GameEngine::GameEngine()
        : running(false), deltaTime(0.0f) {
    }

    void GameEngine::run() {
        initialize();

        while (running && window.isOpen()) {
            deltaTime = clock.restart().asSeconds();

            deltaTime = std::min(deltaTime, TIME_STEP * 3.0f);

            processEvents();
            update();
            render();
        }

        cleanup();
    }

    void GameEngine::initialize() {
        initializeWindow();
        loadAssets();
        initializeSystems();
        initializeStates();

        running = true;
    }

    void GameEngine::initializeWindow() {
        window.create(
            sf::VideoMode(Config::Window::WIDTH, Config::Window::HEIGHT),
            Config::Window::TITLE,
            sf::Style::Titlebar | sf::Style::Close
        );

        window.setFramerateLimit(static_cast<unsigned int>(TARGET_FPS));
        //window.setVerticalSyncEnabled(true);

        try {
            sf::Image icon = AssetManager::getInstance().getImage("icon.png");
            window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        }
        catch (...) {}
    }

    void GameEngine::loadAssets() {
        if (!AssetManager::getInstance().preloadAll()) {
            std::cerr << "Warning: Some assets failed to load" << std::endl;
        }
    }

    void GameEngine::initializeSystems() {
        renderSystem = std::make_unique<RenderSystem>(window);

        // Инициализация звуковой системы
        SoundManager::getInstance().setMusicVolume(50.0f);
        SoundManager::getInstance().setSoundVolume(70.0f);
    }

    void GameEngine::initializeStates() {
        auto mainMenu = std::make_unique<MainMenuState>(*this);
        stateMachine.pushState(std::move(mainMenu));
    }

    void GameEngine::processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                quit();
            }

            stateMachine.handleEvent(event);
        }
    }

    void GameEngine::update() {
        if (!stateMachine.isEmpty()) {
            stateMachine.update(deltaTime);
        }
    }

    void GameEngine::render() {
        renderSystem->clear();

        // Передаем управление рендерингом состоянию
        stateMachine.render(window);

        // Только если состояние не использует renderSystem
        renderSystem->display();
    }

    void GameEngine::cleanup() {
        stateMachine.clearStates();
        SoundManager::getInstance().cleanup();
        AssetManager::getInstance().clear();
    }

    void GameEngine::quit() {
        running = false;
    }

    bool GameEngine::isRunning() const {
        return running;
    }

    StateMachine& GameEngine::getStateMachine() {
        return stateMachine;
    }

    RenderSystem& GameEngine::getRenderSystem() {
        return *renderSystem;
    }

    sf::RenderWindow& GameEngine::getWindow() {
        return window;
    }

    float GameEngine::getDeltaTime() const {
        return deltaTime;
    }

} 
