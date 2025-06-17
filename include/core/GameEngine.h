#pragma once
#include <SFML/Graphics.hpp>
#include "StateMachine.h"
#include "RenderSystem.h"
#include "SoundManager.h"
#include "AssetManager.h"
#include "Config.h"
#include <memory>

namespace Arkanoid {

    class GameEngine {
    public:
        GameEngine();
        ~GameEngine() = default;

        void run();
        void initialize();
        void cleanup();

        // Управление игрой
        void quit();
        bool isRunning() const;

        // Доступ к системам
        StateMachine& getStateMachine();
        RenderSystem& getRenderSystem();
        sf::RenderWindow& getWindow();

        float getDeltaTime() const;

    private:
        sf::RenderWindow window;
        std::unique_ptr<RenderSystem> renderSystem;
        StateMachine stateMachine;

        bool running;
        sf::Clock clock;
        float deltaTime;

        static const float TARGET_FPS;
        static const float TIME_STEP;

        // Методы игрового цикла
        void processEvents();
        void update();
        void render();

        // Инициализация
        void initializeWindow();
        void initializeSystems();
        void initializeStates();
        void loadAssets();
    };

} // namespace Arkanoid
