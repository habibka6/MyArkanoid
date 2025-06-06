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
        powerUpManager.setExtraLifeCallback([this]() {lives = lives + 1;});
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

        if (gameStatus == GameStatus::Playing) {
            inputSystem.update();
            updateGame(deltaTime);
            gameTimer += deltaTime;
        }
    }

    void GameState::render(sf::RenderWindow& window) {
        
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
            break;
        }
    }

    void GameState::handleEvent(const sf::Event& event) {
        inputSystem.processEvent(event);
    }

    void GameState::initializeGame() {
        ball = std::make_unique<Ball>(Config::Window::WIDTH / 2.0f,  Config::Window::HEIGHT - 150.0f );

        paddle = std::make_unique<Paddle>(Config::Window::WIDTH / 2.0f - Config::Paddle::WIDTH / 2.0f,Config::Window::HEIGHT - 50.0f);

        levelManager = std::make_unique<LevelManager>();
    }

    void GameState::initializeInputBindings() {

        inputSystem.bindKey(sf::Keyboard::Left,
            std::make_unique<LambdaCommand>([this]() {
                paddle->move({ -paddle->getSpeed() * engine.getDeltaTime(), 0.0f });
                }));

        inputSystem.bindKey(sf::Keyboard::Right,
            std::make_unique<LambdaCommand>([this]() {
                paddle->move({ paddle->getSpeed() * engine.getDeltaTime(), 0.0f });
                }));

        inputSystem.bindKeyPress(sf::Keyboard::Space,
            std::make_unique<LaunchBallCommand>(*ball));

        inputSystem.bindKeyPress(sf::Keyboard::P,
            std::make_unique<PauseGameCommand>([this]() { pauseGame(); }));

        inputSystem.bindKeyPress(sf::Keyboard::Escape,
            std::make_unique<LambdaCommand>([this]() {
                returnToMainMenu();
                }));;


        inputSystem.bindKeyPress(sf::Keyboard::R,
            std::make_unique<RestartLevelCommand>([this]() { handleLevelRestart(); }));

        inputSystem.bindKeyPress(sf::Keyboard::Enter,
            std::make_unique<LambdaCommand>([this]() {
                if (gameStatus == GameStatus::LevelComplete) {
                    nextLevel();
                }
                }));
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
        if (ball->getIsOnPaddle()) {
            sf::Vector2f paddlePos = paddle->getPosition();
            float paddleCenterX = paddlePos.x + paddle->getBounds().width / 2.0f;
            ball->updateOnPaddle(paddleCenterX, paddlePos.y);
        }
        ball->update(deltaTime);  
        paddle->update(deltaTime);
        powerUpManager.update(deltaTime, *paddle, *ball);
        paddle->constrainToWindow(static_cast<float>(Config::Window::WIDTH));

        if (!ball->getIsOnPaddle()) {
            physicsSystem->update(*ball, *paddle, blocks, powerups, deltaTime);
        }
        checkGameConditions();
        cleanupInactiveObjects();
    }

    
    void GameState::checkGameConditions() {
        if (!ball->getIsOnPaddle() && ball->getPosition().y > Config::Window::HEIGHT) {
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
        powerUpManager.reset();
        loadLevel(currentLevel);
        gameStatus = GameStatus::Playing;
        
    }

    void GameState::handleLevelRestart() {
        if (gameStatus == GameStatus::GameOver) {
            restartLevel();
        }
    }

    void GameState::returnToMainMenu() {
        SoundManager::getInstance().playSound(SoundType::ButtonClick);
        auto mainMenu = std::make_unique<MainMenuState>(engine);
        engine.getStateMachine().changeState(std::move(mainMenu));
    }



    void GameState::nextLevel() {
        if (levelManager->hasNextLevel()) {
            levelManager->nextLevel();
            powerUpManager.reset();
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
        sf::Vector2f paddlePos = paddle->getPosition();
        float paddleCenterX = paddlePos.x + paddle->getBounds().width / 2.0f;
        ball->reset(paddleCenterX, paddlePos.y);
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
            SoundManager::getInstance().playSound(SoundType::LoseBall);
            resetBall();
        }
    }

    bool GameState::isLevelComplete() const {
        return levelManager->isLevelComplete(blocks);
    }

    void GameState::cleanupInactiveObjects() {
        auto powerupIt = powerups.begin();
        while (powerupIt != powerups.end()) {
            try {
                if (*powerupIt && !(*powerupIt)->isActive()) {
                    powerupIt = powerups.erase(powerupIt);
                }
                else {
                    ++powerupIt;
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error checking powerup state: " << e.what() << std::endl;
                powerupIt = powerups.erase(powerupIt);
            }
        }

        auto blockIt = blocks.begin();
        while (blockIt != blocks.end()) {
            try {
                if (*blockIt && !(*blockIt)->isActive()) {
                    blockIt = blocks.erase(blockIt);
                }
                else {
                    ++blockIt;
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error checking block state: " << e.what() << std::endl;
                blockIt = blocks.erase(blockIt); 
            }
        }
    }

} 
