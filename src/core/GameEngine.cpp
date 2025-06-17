#include "GameEngine.h"
#include "MainMenuState.h"

namespace Arkanoid {

    const float GameEngine::TARGET_FPS = Config::Game::TARGET_FPS;
    const float GameEngine::TIME_STEP = 1.0f / TARGET_FPS;

    GameEngine::GameEngine() : running(false), deltaTime(0.0f) {}

    void GameEngine::run()
    {
        initialize();

        while (running && window.isOpen())
        {
            deltaTime = std::min(clock.restart().asSeconds(), TIME_STEP * 3.0f);

            processEvents();
            update();
            render();
        }

        cleanup();
    }

    void GameEngine::initialize()
    {
        initializeWindow();
        loadAssets();
        initializeSystems();
        initializeStates();
        running = true;
    }

    void GameEngine::initializeWindow()
    {
        window.create(
            sf::VideoMode(Config::Window::WIDTH, Config::Window::HEIGHT),
            Config::Window::TITLE,
            sf::Style::Titlebar | sf::Style::Close
        );

        window.setFramerateLimit(static_cast<unsigned int>(TARGET_FPS));
        window.setVerticalSyncEnabled(true);

        try {
            const auto& icon = AssetManager::getInstance().getImage("icon.png");
            window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        }
        catch (...) {}
    }

    void GameEngine::loadAssets()
    {
        AssetManager::getInstance().preloadAll();
    }

    void GameEngine::initializeSystems()
    {
        renderSystem = std::make_unique<RenderSystem>(window);
        SoundManager::getInstance().setMusicVolume(50.0f);
        SoundManager::getInstance().setSoundVolume(70.0f);
    }

    void GameEngine::initializeStates()
    {
        stateMachine.pushState(std::make_unique<MainMenuState>(*this));
    }

    void GameEngine::processEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                quit();
            }
            stateMachine.handleEvent(event);
        }
    }

    void GameEngine::update()
    {
        if (!stateMachine.isEmpty()) {
            stateMachine.update(deltaTime);
        }
    }

    void GameEngine::render()
    {
        renderSystem->clear();
        stateMachine.render(window);
        renderSystem->display();
    }

    void GameEngine::cleanup()
    {
        stateMachine.clearStates();
        SoundManager::getInstance().cleanup();
        AssetManager::getInstance().clear();
    }

    void GameEngine::quit()
    {
        running = false;
    }

    bool GameEngine::isRunning() const
    {
        return running;
    }

    StateMachine& GameEngine::getStateMachine()
    {
        return stateMachine;
    }

    RenderSystem& GameEngine::getRenderSystem()
    {
        return *renderSystem;
    }

    sf::RenderWindow& GameEngine::getWindow()
    {
        return window;
    }

    float GameEngine::getDeltaTime() const
    {
        return deltaTime;
    }

} // namespace Arkanoid