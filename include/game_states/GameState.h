﻿// Основное игровое состояние, управляющее игровым процесссом
#pragma once
#include "State.h"
#include "Ball.h"
#include "Paddle.h"
#include "BaseBlock.h"
#include "PowerUp.h"
#include "LevelManager.h"
#include "PhysicsSystem.h"
#include "PowerUpManager.h"
#include "MainMenuState.h"
#include "SoundManager.h"
#include "ICollisionObserver.h"
#include <vector>
#include <memory>

namespace Arkanoid {

    class GameState : public State, public ICollisionObserver {
    public:
        GameState(GameEngine& engine, int startingLevel = 1);
        ~GameState() override = default;

        // Управление состоянием
        void enter() override;
        void exit() override;
        void pause() override;
        void resume() override;
        void update(float deltaTime) override;
        void render(sf::RenderWindow& window) override;
        void handleEvent(const sf::Event& event) override;

        // Обработка коллизий по коллбэку (Обработка игровых последствий : очки, звуки)
        void onCollision(CollisionType type, Entity* obj1, Entity* obj2) override;

    private:
        // Основные игровые объекты
        std::unique_ptr<Ball> ball;
        std::unique_ptr<Paddle> paddle;
        std::vector<std::unique_ptr<BaseBlock>> blocks;
        std::vector<std::unique_ptr<PowerUp>> powerups;

        // Подсистемы
        std::unique_ptr<PhysicsSystem> physicsSystem;
        std::unique_ptr<LevelManager> levelManager;
        PowerUpManager powerUpManager;

        // Игровые параметры
        GameStatus gameStatus = GameStatus::Playing;
        int score = 0;
        int lives = Config::Game::LIVES;
        int currentLevel = 1;
        float gameTimer = 0.0f;

        // Состояние ввода
        bool leftPressed = false;
        bool rightPressed = false;
        bool launchPressed = false;

        void initializeGame();
        void initializePhysics();
        void loadLevel(int levelNumber);

        //Обновление игровой логики и проврека состояния игры.
        void updateGame(float deltaTime);
        void checkGameConditions();

        // Управление игровым процессом
        void pauseGame();
        void restartLevel();
        void nextLevel();
        void gameOver();
        void returnToMainMenu();


        void resetBall();
        void updateScore(int points);
        void loseLife();
        bool isLevelComplete() const;
        void cleanupInactiveObjects();

        // Обработка ввода (управление платформой)
        void processKeyboardInput(float deltaTime);
    };

}
