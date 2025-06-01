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

    enum class GameStatus {
        Playing,
        Paused,
        LevelComplete,
        GameOver,
        Victory
    };

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

        // UI ��������
        sf::Font font;
        sf::Text pauseText;
        sf::Text gameOverText;
        sf::Text levelCompleteText;

        // ���������
        bool debugMode;

        // ������ �������������
        void initializeGame();
        void initializeInputBindings();
        void initializePhysics();
        void initializeUI();
        void loadLevel(int levelNumber);

        // ������� ������
        void updateGame(float deltaTime);
        void updatePowerUps(float deltaTime);
        void checkGameConditions();
        void handleCollisions();

        // ���������� ����������
        void pauseGame();
        void resumeGame();
        void restartLevel();
        void nextLevel();
        void gameOver();

        // ���������
        void renderGame(sf::RenderWindow& window);
        void renderPauseScreen(sf::RenderWindow& window);
        void renderGameOverScreen(sf::RenderWindow& window);
        void renderLevelCompleteScreen(sf::RenderWindow& window);
        void renderDebugInfo(sf::RenderWindow& window);

        // ������� ������
        void onBallBlockCollision(Ball& ball, BaseBlock& block);
        void onBallPaddleCollision(Ball& ball, Paddle& paddle);
        void onBallWallCollision(Ball& ball);
        void onPowerUpCollected(BasePowerUp& powerup);

        // �������
        void resetBall();
        void updateScore(int points);
        void loseLife();
        bool isLevelComplete() const;
        void cleanupInactiveObjects();
    };

} // namespace Arkanoid
