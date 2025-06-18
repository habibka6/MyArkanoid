#include "GameState.h"
#include "GameEngine.h"
#include "GameOverState.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

namespace Arkanoid {

    GameState::GameState(GameEngine& engine, int startingLevel)
        : State(engine),
        gameStatus(GameStatus::Playing),
        score(0),
        lives(3),
        currentLevel(startingLevel),
        gameTimer(0.0f),
        powerUpManager(powerups)
    {
        powerUpManager.setExtraLifeCallback([this]() { lives++; }); 
    }

    // Вход в игровое состояние
    void GameState::enter() {
        initializeGame();        
        initializePhysics();      
        loadLevel(currentLevel); 
        SoundManager::getInstance().playMusic("game_music.ogg", true); //нужно загрузить аудиофайл в "music/" (сейчас там глушилка стоит)
    }

    // Выход из игрового состояния
    void GameState::exit() {
        SoundManager::getInstance().stopMusic(); 
    }

    

    // Обновление состояния игры
    void GameState::update(float deltaTime) {
        if (gameStatus == GameStatus::Playing) {
            processKeyboardInput(deltaTime); 
            updateGame(deltaTime);           
            gameTimer += deltaTime;          
        }
    }

    // Обновление игровой логики
    void GameState::updateGame(float deltaTime) {
        cleanupInactiveObjects();
        physicsSystem->update(*ball, *paddle, blocks, powerups, deltaTime); 

        if (ball->getIsOnPaddle()) {
            auto paddlePos = paddle->getPosition();
            float paddleCenterX = paddlePos.x + paddle->getBounds().width / 2.0f;
            ball->updateOnPaddle(paddleCenterX, paddlePos.y);
        }

        ball->update(deltaTime);              
        paddle->update(deltaTime);              
        powerUpManager.update(deltaTime, *paddle, *ball); 
        paddle->constrainToWindow(static_cast<float>(Config::Window::WIDTH)); 

        checkGameConditions();
    }


    // Проверка условий завершения уровня или потери жизни
    void GameState::checkGameConditions() {
        if (!ball->getIsOnPaddle() && ball->getPosition().y > Config::Window::HEIGHT) {
            loseLife();
        }

        if (isLevelComplete()) {
            gameStatus = GameStatus::LevelComplete; 
            SoundManager::getInstance().playSound(SoundType::LevelComplete);
        }
    }



    // Отрисовка игрового состояния
    void GameState::render(sf::RenderWindow& window) {
        engine.getRenderSystem().renderGameContent(
            *ball, *paddle, blocks, powerups,
            powerUpManager.getActiveEffects(),
            score, lives, currentLevel
        );
        powerUpManager.render(window);

        // Оверлеи для паузы и завершения уровня
        switch (gameStatus) {
        case GameStatus::Paused:
            engine.getRenderSystem().renderPauseOverlay();
            break;
        case GameStatus::LevelComplete:
            engine.getRenderSystem().renderLevelCompleteOverlay();
            break;
        default:
            break;
        }
    }

    // Обработка событий клавиатуры
    void GameState::handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
            case sf::Keyboard::Left:   leftPressed = true; break;
            case sf::Keyboard::Right:  rightPressed = true; break;
            case sf::Keyboard::Space:  launchPressed = true; break;
            case sf::Keyboard::P:      pauseGame(); break;
            case sf::Keyboard::Escape: returnToMainMenu(); break;
            case sf::Keyboard::Enter:
                if (gameStatus == GameStatus::LevelComplete) nextLevel();
                break;
            default: break;
            }
        }
        else if (event.type == sf::Event::KeyReleased) {
            switch (event.key.code) {
            case sf::Keyboard::Left:   leftPressed = false; break;
            case sf::Keyboard::Right:  rightPressed = false; break;
            case sf::Keyboard::Space:  launchPressed = false; break;
            default: break;
            }
        }
    }

    // Обработка нажатий для движения и запуска мяча
    void GameState::processKeyboardInput(float deltaTime) {
        float moveDistance = paddle->getSpeed() * deltaTime;
        if (leftPressed)  paddle->move({ -moveDistance, 0.0f });
        if (rightPressed) paddle->move({ moveDistance, 0.0f });
        if (launchPressed && ball->getIsOnPaddle()) {
            ball->launch();   
            launchPressed = false;
        }
    }

    // Реакция на игровые коллизии (Observer)
    void GameState::onCollision(CollisionType type, Entity* obj1, Entity* obj2) {
        switch (type) {
        case CollisionType::BallBlock: {
            auto* ballPtr = dynamic_cast<Ball*>(obj1);
            auto* block = dynamic_cast<BaseBlock*>(obj2);
            if (ballPtr && block) {
                block->hit();
                SoundManager::getInstance().playSound(
                    block->getBlockType() == BlockType::Rock ? SoundType::RockHit : SoundType::BlockHit
                );
                if (block->isDestroyed()) {
                    updateScore(block->getPoints()); // Очки за сбор бонуса
                    powerUpManager.spawnPowerUp(block->getPosition()); 
                }
            }
            break;
        }
        case CollisionType::BallPaddle: {
            auto* ballPtr = dynamic_cast<Ball*>(obj1);
            auto* paddlePtr = dynamic_cast<Paddle*>(obj2);
            if (ballPtr && paddlePtr) {
                SoundManager::getInstance().playSound(SoundType::PaddleHit);
            }
            break;
        }
        case CollisionType::BallWall: {
            auto* ballPtr = dynamic_cast<Ball*>(obj1);
            if (ballPtr) {
                SoundManager::getInstance().playSound(SoundType::WallHit);
            }
            break;
        }
        case CollisionType::PaddlePowerUp: {
            auto* powerup = dynamic_cast<PowerUp*>(obj2);
            if (powerup) {
                SoundManager::getInstance().playSound(SoundType::PowerUpCollect);
                updateScore(50); // Очки за сбор бонуса
                powerUpManager.applyPowerUpEffect(*powerup, *paddle, *ball); 
            }
            break;
        }
        }
    }

    // Создание игровых объектов
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
        powerUpManager.setExtraLifeCallback([this]() { lives++; });
    }

    // Инициализация физической системы и подписка на события
    void GameState::initializePhysics() {
        physicsSystem = std::make_unique<PhysicsSystem>(engine.getWindow());
        physicsSystem->addObserver(this); 
    }

    // Загрузка уровня
    void GameState::loadLevel(int levelNumber) {
        if (levelManager->loadLevel(levelNumber)) {
            blocks = levelManager->generateBlocks();
            currentLevel = levelNumber;
            resetBall(); 
        }
    }

    // Пауза игры
    void GameState::pause() {
        gameStatus = GameStatus::Paused;
        SoundManager::getInstance().pauseMusic();
    }

    // Возобновление игры после паузы
    void GameState::resume() {
        if (gameStatus == GameStatus::Paused) {
            gameStatus = GameStatus::Playing;
            SoundManager::getInstance().resumeMusic();
        }
    }

    // Переключение паузы
    void GameState::pauseGame() {
        if (gameStatus == GameStatus::Playing) {
            gameStatus = GameStatus::Paused;
        }
        else if (gameStatus == GameStatus::Paused) {
            gameStatus = GameStatus::Playing;
        }
    }

    // Перезапуск текущего уровня
    void GameState::restartLevel() {
        loadLevel(currentLevel);
        gameStatus = GameStatus::Playing;
    }

    // Переход к следующему уровню или победа
    void GameState::nextLevel() {
        if (levelManager->hasNextLevel()) {
            levelManager->nextLevel();
            powerUpManager.reset();
            loadLevel(levelManager->getCurrentLevelNumber());
            gameStatus = GameStatus::Playing;
        }
        else {
            auto gameOverState = std::make_unique<GameOverState>(
                engine, GameOverReason::Victory, score, currentLevel
            );
            engine.getStateMachine().changeState(std::move(gameOverState));
        }
    }

    // Завершение игры 
    void GameState::gameOver() {
        auto gameOverState = std::make_unique<GameOverState>(
            engine, GameOverReason::Defeat, score, currentLevel
        );
        engine.getStateMachine().changeState(std::move(gameOverState));
    }

    // Возврат в главное меню
    void GameState::returnToMainMenu() {
        SoundManager::getInstance().playSound(SoundType::ButtonClick);
        auto mainMenu = std::make_unique<MainMenuState>(engine);
        engine.getStateMachine().changeState(std::move(mainMenu));
    }


    // Сброс мяча на платформу
    void GameState::resetBall() {
        auto paddlePos = paddle->getPosition();
        float paddleCenterX = paddlePos.x + paddle->getBounds().width / 2.0f;
        ball->reset(paddleCenterX, paddlePos.y);
    }

    // Увеличение счета
    void GameState::updateScore(int points) {
        score += points;
    }

    // Потеря жизни игроком
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

    // Проверка завершения уровня
    bool GameState::isLevelComplete() const {
        return levelManager->isLevelComplete(blocks);
    }

    // Очистка неактивных объектов (блоки, бонусы)
    void GameState::cleanupInactiveObjects() {
        powerups.erase(
            std::remove_if(powerups.begin(), powerups.end(),
                [](const std::unique_ptr<PowerUp>& p) {
                    return !p || !p->isActive();
                }),
            powerups.end()
        );
        blocks.erase(
            std::remove_if(blocks.begin(), blocks.end(),
                [](const auto& b) {
                    return !b || !b->isActive();
                }),
            blocks.end()
        );
    }

} // namespace Arkanoid
