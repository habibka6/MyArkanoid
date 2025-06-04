#include "GameState.h"
#include "GameEngine.h"
#include "GameOverState.h"
#include "AssetManager.h"
#include "SoundManager.h"
#include <iostream>

namespace Arkanoid {

    GameState::GameState(GameEngine& engine, int startingLevel)
        : State(engine),
        gameStatus(GameStatus::Playing),
        score(0),
        lives(3),
        currentLevel(startingLevel),
        gameTimer(0.0f),
        debugMode(false)
    {
        powerUpManager.setExtraLifeCallback([this]() {
            lives = lives + 1;
            });
    }

    void GameState::enter() {
        initializeGame();
        initializeInputBindings();
        initializePhysics();
        loadLevel(currentLevel);

        SoundManager::getInstance().playMusic("game_music.ogg", true);
    }

    void GameState::exit() {
        SoundManager::getInstance().stopMusic();
    }

    void GameState::pause() {
        gameStatus = GameStatus::Paused;
        SoundManager::getInstance().pauseMusic();
    }

    void GameState::resume() {
        if (gameStatus == GameStatus::Paused) {
            gameStatus = GameStatus::Playing;
            SoundManager::getInstance().resumeMusic();
        }
    }

    void GameState::update(float deltaTime) {
        inputSystem.update();
        if (gameStatus == GameStatus::Playing) {
            updateGame(deltaTime);
            gameTimer += deltaTime;
        }
    }

    void GameState::render(sf::RenderWindow& window) {
        // Используем RenderSystem для отрисовки игрового контента
        engine.getRenderSystem().renderGameContent(
            *ball,
            *paddle,
            blocks,
            powerups,
            powerUpManager.getActiveEffects(),
            score,
            lives,
            currentLevel
        );
        powerUpManager.render(window);

        // Рендеринг оверлеев состояний поверх игрового контента
        switch (gameStatus) {
        case GameStatus::Paused:
            engine.getRenderSystem().renderPauseOverlay();
            break;
        case GameStatus::LevelComplete:
            engine.getRenderSystem().renderLevelCompleteOverlay();
            break;
        case GameStatus::GameOver:
            engine.getRenderSystem().renderGameOverOverlay();
            break;
        case GameStatus::Playing:
        case GameStatus::Victory:
        default:
            // Никаких дополнительных оверлеев
            break;
        }

        // Отладочная информация
        if (debugMode) {
            engine.getRenderSystem().renderDebugInfo(*ball, *paddle, gameTimer);
        }
    }

    // Остальные методы остаются без изменений...
    void GameState::handleEvent(const sf::Event& event) {
        inputSystem.processEvent(event);

        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
            case sf::Keyboard::Escape:
                pauseGame();
                break;
            case sf::Keyboard::R:
                if (gameStatus == GameStatus::GameOver) {
                    restartLevel();
                }
                break;
            case sf::Keyboard::Enter:
                if (gameStatus == GameStatus::LevelComplete) {
                    nextLevel();
                }
                break;
            case sf::Keyboard::F1:
                debugMode = !debugMode;
                break;
            }
        }
    }

    void GameState::initializeGame() {
        ball = std::make_unique<Ball>(
            Config::Window::WIDTH / 2.0f,
            Config::Window::HEIGHT - 150.0f
        );

        paddle = std::make_unique<Paddle>(
            Config::Window::WIDTH / 2.0f - Config::Paddle::WIDTH / 2.0f,
            Config::Window::HEIGHT - 50.0f
        );

        levelManager = std::make_unique<LevelManager>();
    }

    void GameState::initializeInputBindings() {
        inputSystem.bindKey(sf::Keyboard::Left,
            std::make_unique<PaddleMoveLeftCommand>(*paddle, engine.getDeltaTime()));
        inputSystem.bindKey(sf::Keyboard::Right,
            std::make_unique<PaddleMoveRightCommand>(*paddle, engine.getDeltaTime()));

        inputSystem.bindKeyPress(sf::Keyboard::P,
            std::make_unique<PauseGameCommand>([this]() { pauseGame(); }));
    }

    void GameState::initializePhysics() {
        physicsSystem = std::make_unique<PhysicsSystem>(engine.getWindow());

        physicsSystem->setCollisionCallback([this](CollisionType type, Entity* obj1, Entity* obj2) {
            switch (type) {
            case CollisionType::BallBlock:
                if (auto* ball = dynamic_cast<Ball*>(obj1)) {
                    if (auto* block = dynamic_cast<BaseBlock*>(obj2)) {
                        block->hit();
                        SoundManager::getInstance().playSound(block->getBlockType() == BlockType::Rock ? SoundType::RockHit : SoundType::BlockHit);
                        if (block->isDestroyed()) {
                            updateScore(block->getPoints());
                            powerUpManager.spawnPowerUp(block->getPosition());
                        }
                    }
                }
                break;
            case CollisionType::BallPaddle:
                if (auto* ball = dynamic_cast<Ball*>(obj1)) {
                    if (auto* paddle = dynamic_cast<Paddle*>(obj2)) {
                        SoundManager::getInstance().playSound(SoundType::PaddleHit);
                    }
                }
                break;
            case CollisionType::BallWall:
                if (auto* ball = dynamic_cast<Ball*>(obj1)) {
                    SoundManager::getInstance().playSound(SoundType::WallHit);
                }
                break;
            case CollisionType::PaddlePowerUp:
                if (auto* powerup = dynamic_cast<BasePowerUp*>(obj2)) {
                    SoundManager::getInstance().playSound(SoundType::PowerUpCollect);
                    updateScore(50);
                }
                break;
            }
            });
    }

    void GameState::loadLevel(int levelNumber) {
        if (levelManager->loadLevel(levelNumber)) {
            blocks = levelManager->generateBlocks();
            currentLevel = levelNumber;
            resetBall();
        }
    }

    void GameState::updateGame(float deltaTime) {
        ball->update(deltaTime);
        paddle->update(deltaTime);
        powerUpManager.update(deltaTime, *paddle, *ball);
        paddle->constrainToWindow(static_cast<float>(Config::Window::WIDTH));
        physicsSystem->update(*ball, *paddle, blocks, powerups, deltaTime);
        checkGameConditions();
        cleanupInactiveObjects();
    }

    
    void GameState::checkGameConditions() {
        if (ball->getPosition().y > Config::Window::HEIGHT) {
            loseLife();
        }

        if (isLevelComplete()) {
            gameStatus = GameStatus::LevelComplete;
            SoundManager::getInstance().playSound(SoundType::LevelComplete);
        }
    }
    void GameState::pauseGame() {
        if (gameStatus == GameStatus::Playing) {
            gameStatus = GameStatus::Paused;
        }
        else if (gameStatus == GameStatus::Paused) {
            gameStatus = GameStatus::Playing;
        }
    }

    void GameState::resumeGame() {
        gameStatus = GameStatus::Playing;
    }

    void GameState::restartLevel() {
        loadLevel(currentLevel);
        gameStatus = GameStatus::Playing;
        powerUpManager.reset();
    }

    void GameState::nextLevel() {
        if (levelManager->hasNextLevel()) {
            levelManager->nextLevel();
            loadLevel(levelManager->getCurrentLevelNumber());
            gameStatus = GameStatus::Playing;
        }
        else {
            auto gameOverState = std::make_unique<GameOverState>(
                engine, GameOverReason::Victory, score, currentLevel);
            engine.getStateMachine().changeState(std::move(gameOverState));
        }
    }

    void GameState::gameOver() {
        gameStatus = GameStatus::GameOver;
        SoundManager::getInstance().playSound(SoundType::GameOver);

        auto gameOverState = std::make_unique<GameOverState>(
            engine, GameOverReason::Defeat, score, currentLevel);
        engine.getStateMachine().changeState(std::move(gameOverState));
    }

    void GameState::resetBall() {
        ball->reset(Config::Window::WIDTH / 2.0f, Config::Window::HEIGHT - 150.0f);
    }

    void GameState::updateScore(int points) {
        score += points;
    }

    void GameState::loseLife() {
        lives--;
        if (lives <= 0) {
            gameOver();
        }
        else {
            resetBall();
        }
    }

    bool GameState::isLevelComplete() const {
        return levelManager->isLevelComplete(blocks);
    }

    void GameState::cleanupInactiveObjects() {
        powerups.erase(
            std::remove_if(powerups.begin(), powerups.end(),
                [](const std::unique_ptr<BasePowerUp>& powerup) {
                    return !powerup->isActive();
                }),
            powerups.end()
        );

        blocks.erase(
            std::remove_if(blocks.begin(), blocks.end(),
                [](const std::unique_ptr<BaseBlock>& block) {
                    return !block->isActive();
                }),
            blocks.end()
        );
    }

} // namespace Arkanoid
