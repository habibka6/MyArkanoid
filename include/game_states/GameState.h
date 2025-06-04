#pragma once
#include "State.h"
#include "Ball.h"
#include "Paddle.h"
#include "BaseBlock.h"
#include "BasePowerUp.h"
#include "LevelManager.h"
#include "PhysicsSystem.h"
#include "PowerUpManager.h"
#include "InputSystem.h"
#include "SoundManager.h"
#include <vector>
#include <memory>
#include <random>

namespace Arkanoid {

    enum class GameStatus;

    class GameState : public State {
    public:
        GameState(GameEngine& engine, int startingLevel = 1);
        ~GameState() = default;

        void update(float deltaTime) override;
        void render(sf::RenderWindow& window) override;
        void handleEvent(const sf::Event& event) override;

        void enter() override;
        void exit() override;
        void pause() override;
        void resume() override;

    private:
        // ������� �������
        std::unique_ptr<Ball> ball;
        std::unique_ptr<Paddle> paddle;
        std::vector<std::unique_ptr<BaseBlock>> blocks;
        std::vector<std::unique_ptr<BasePowerUp>> powerups;

        // �������
        std::unique_ptr<PhysicsSystem> physicsSystem;
        std::unique_ptr<LevelManager> levelManager;
        InputSystem inputSystem;
        PowerUpManager powerUpManager;

        // ������� ���������
        GameStatus gameStatus;
        int score;
        int lives;
        int currentLevel;
        float gameTimer;

        // ���������
        bool debugMode;

        // ������ �������������
        void initializeGame();
        void initializeInputBindings();
        void initializePhysics();
        void loadLevel(int levelNumber);

        // ������� ������
        void updateGame(float deltaTime);
    
        void checkGameConditions();

        // ���������� ����������
        void pauseGame();
        void resumeGame();
        void restartLevel();
        void nextLevel();
        void gameOver();

        // �������
        void resetBall();
        void updateScore(int points);
        void loseLife();
        bool isLevelComplete() const;
        void cleanupInactiveObjects();
    };

} // namespace Arkanoid
