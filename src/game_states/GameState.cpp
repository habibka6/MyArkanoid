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
        gameTimer(0.0f),debugMode(false) 
    {
        powerUpManager.setExtraLifeCallback([this]() {
            lives = lives + 1;
            });
    }

    void GameState::enter() {
        initializeGame();
        initializeInputBindings();
        initializePhysics();
        initializeUI();
        loadLevel(currentLevel);

        // Запускаем игровую музыку
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
        switch (gameStatus) {
        case GameStatus::Playing:
            renderGame(window);
            break;
        case GameStatus::Paused:
            renderGame(window);
            renderPauseScreen(window);
            break;
        case GameStatus::LevelComplete:
            renderGame(window);
            renderLevelCompleteScreen(window);
            break;
        case GameStatus::GameOver:
            renderGame(window);
            renderGameOverScreen(window);
            break;
        }

        if (debugMode) {
            renderDebugInfo(window);
        }
    }

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
        // Создаем игровые объекты
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
        // Привязываем управление платформой
        inputSystem.bindKey(sf::Keyboard::Left,
            std::make_unique<PaddleMoveLeftCommand>(*paddle, engine.getDeltaTime()));
        inputSystem.bindKey(sf::Keyboard::Right,
            std::make_unique<PaddleMoveRightCommand>(*paddle, engine.getDeltaTime()));

        // Привязываем паузу
        inputSystem.bindKeyPress(sf::Keyboard::P,
            std::make_unique<PauseGameCommand>([this]() { pauseGame(); }));
    }

    void GameState::initializePhysics() {
        physicsSystem = std::make_unique<PhysicsSystem>(engine.getWindow());

        // Устанавливаем колбэки физики
        physicsSystem->setCollisionCallback([this](CollisionType type, Entity* obj1, Entity* obj2) {
            switch (type) {
            case CollisionType::BallBlock:
                if (auto* ball = dynamic_cast<Ball*>(obj1)) {
                    if (auto* block = dynamic_cast<BaseBlock*>(obj2)) {
                        onBallBlockCollision(*ball, *block);
                    }
                }
                break;
            case CollisionType::BallPaddle:
                if (auto* ball = dynamic_cast<Ball*>(obj1)) {
                    if (auto* paddle = dynamic_cast<Paddle*>(obj2)) {
                        onBallPaddleCollision(*ball, *paddle);
                    }
                }
                break;
            case CollisionType::BallWall:
                if (auto* ball = dynamic_cast<Ball*>(obj1)) {
                    onBallWallCollision(*ball);
                }
                break;
            case CollisionType::PaddlePowerUp:
                if (auto* powerup = dynamic_cast<BasePowerUp*>(obj2)) {
                    onPowerUpCollected(*powerup);
                }
                break;
            }
            });
    }

    void GameState::initializeUI() {
        try {
            font = AssetManager::getInstance().getFont("retro.ttf");

            pauseText.setFont(font);
            pauseText.setString("PAUSED");
            pauseText.setCharacterSize(72);
            pauseText.setFillColor(sf::Color::Yellow);

            gameOverText.setFont(font);
            gameOverText.setString("GAME OVER");
            gameOverText.setCharacterSize(72);
            gameOverText.setFillColor(sf::Color::Red);

            levelCompleteText.setFont(font);
            levelCompleteText.setString("LEVEL COMPLETE!");
            levelCompleteText.setCharacterSize(48);
            levelCompleteText.setFillColor(sf::Color::Green);

        }
        catch (const std::exception& e) {
            std::cerr << "Error initializing UI: " << e.what() << std::endl;
        }
    }

    void GameState::loadLevel(int levelNumber) {
        if (levelManager->loadLevel(levelNumber)) {
            blocks = levelManager->generateBlocks();
            currentLevel = levelNumber;
            resetBall();
        }
    }

    void GameState::updateGame(float deltaTime) {
        // Обновляем игровые объекты
        ball->update(deltaTime);
        paddle->update(deltaTime);
        updatePowerUps(deltaTime);
        powerUpManager.update(deltaTime, *paddle, *ball);
        // Ограничиваем платформу границами окна
        paddle->constrainToWindow(static_cast<float>(Config::Window::WIDTH));

        // Обновляем физику
        physicsSystem->update(*ball, *paddle, blocks, powerups, deltaTime);

        // Проверяем условия игры
        checkGameConditions();

        // Очищаем неактивные объекты
        cleanupInactiveObjects();
    }

    void GameState::updatePowerUps(float deltaTime) {
        for (auto& powerup : powerups) {
            if (powerup->isActive()) {
                powerup->update(deltaTime);

                // Удаляем бонусы, вышедшие за экран
                if (powerup->isOutOfBounds(static_cast<float>(Config::Window::HEIGHT))) {
                    powerup->setActive(false);
                }
            }
        }
    }

    void GameState::checkGameConditions() {
        // Проверяем потерю мяча
        if (ball->getPosition().y > Config::Window::HEIGHT) {
            loseLife();
        }

        // Проверяем завершение уровня
        if (isLevelComplete()) {
            gameStatus = GameStatus::LevelComplete;
            SoundManager::getInstance().playSound(SoundType::LevelComplete);
        }
    }

    void GameState::renderGame(sf::RenderWindow& window) {
        // Рендерим игровые объекты
        ball->draw(window);
        paddle->draw(window);

        for (const auto& block : blocks) {
            if (block->isActive()) {
                block->draw(window);
            }
        }

        for (const auto& powerup : powerups) {
            if (powerup->isActive()) {
                powerup->draw(window);
            }
        }
        // Рендер бонусов
        powerUpManager.render(window);

        // Рендер UI с активными эффектами
        engine.getRenderSystem().renderActiveEffects(powerUpManager.getActiveEffects());
        // Рендерим UI
        engine.getRenderSystem().renderUI(score, lives, currentLevel);
    }

    void GameState::renderPauseScreen(sf::RenderWindow& window) {
        // Полупрозрачный оверлей
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(Config::Window::WIDTH, Config::Window::HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 128));
        window.draw(overlay);

        // Центрируем текст паузы
        sf::FloatRect textBounds = pauseText.getLocalBounds();
        pauseText.setPosition(
            (Config::Window::WIDTH - textBounds.width) / 2,
            (Config::Window::HEIGHT - textBounds.height) / 2
        );
        window.draw(pauseText);
    }

    void GameState::renderGameOverScreen(sf::RenderWindow& window) {
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(Config::Window::WIDTH, Config::Window::HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(overlay);

        sf::FloatRect textBounds = gameOverText.getLocalBounds();
        gameOverText.setPosition(
            (Config::Window::WIDTH - textBounds.width) / 2,
            (Config::Window::HEIGHT - textBounds.height) / 2
        );
        window.draw(gameOverText);
    }

    void GameState::renderLevelCompleteScreen(sf::RenderWindow& window) {
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(Config::Window::WIDTH, Config::Window::HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 100));
        window.draw(overlay);

        sf::FloatRect textBounds = levelCompleteText.getLocalBounds();
        levelCompleteText.setPosition(
            (Config::Window::WIDTH - textBounds.width) / 2,
            (Config::Window::HEIGHT - textBounds.height) / 2
        );
        window.draw(levelCompleteText);
    }

    void GameState::renderDebugInfo(sf::RenderWindow& window) {
        // TODO: Добавить отображение отладочной информации
        // Можно показывать FPS, позицию мяча, скорость и т.д.
    }

    void GameState::onBallBlockCollision(Ball& ball, BaseBlock& block) {
        if (block.hit()) {
            updateScore(block.getPoints());
            SoundManager::getInstance().playSound(SoundType::BlockHit);

            // Увеличиваем скорость мяча
            ball.increaseSpeedFactor(Config::Ball::SPEED_INCREASE_PER_HIT);

            // Генерация бонуса только если нет активных временных эффектов
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_real_distribution<float> dis(0.0f, 1.0f);

            // Если нет активных временных эффектов и выпал шанс
            if (!powerUpManager.isTemporaryEffectActive() &&
                dis(gen) < Config::Block::DROP_CHANCE)
            {
                powerUpManager.spawnPowerUp(block.getPosition());
            }
        }

    }

    void GameState::onBallPaddleCollision(Ball& ball, Paddle& paddle) {
        SoundManager::getInstance().playSound(SoundType::PaddleHit);
    }

    void GameState::onBallWallCollision(Ball& ball) {
        SoundManager::getInstance().playSound(SoundType::WallHit);
    }

    void GameState::onPowerUpCollected(BasePowerUp& powerup) {
        SoundManager::getInstance().playSound(SoundType::PowerUpCollect);
        updateScore(50); // Бонус за сбор
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
            // Игра завершена - победа
            auto gameOverState = std::make_unique<GameOverState>(
                engine, GameOverReason::Victory, score, currentLevel);
            engine.getStateMachine().changeState(std::move(gameOverState));
        }
    }

    void GameState::gameOver() {
        gameStatus = GameStatus::GameOver;
        SoundManager::getInstance().playSound(SoundType::GameOver);

        // Переходим к экрану окончания игры
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
        // Удаляем неактивные бонусы
        powerups.erase(
            std::remove_if(powerups.begin(), powerups.end(),
                [](const std::unique_ptr<BasePowerUp>& powerup) {
                    return !powerup->isActive();
                }),
            powerups.end()
        );

        // Удаляем разрушенные блоки
        blocks.erase(
            std::remove_if(blocks.begin(), blocks.end(),
                [](const std::unique_ptr<BaseBlock>& block) {
                    return !block->isActive();
                }),
            blocks.end()
        );
    }

} // namespace Arkanoid
