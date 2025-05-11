#include "GameEngine.h"
#include <sstream>

GameEngine::GameEngine()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Arkanoid"),
    paddle(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT - 50), //Центрирование платформы
    ball(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100) 
{    

    //Загрузка фона и масштабирование под размер окна
    backgroundSprite.setTexture(AssetManager::GetTexture("background.png"));
    
    float scaleX = WINDOW_WIDTH / backgroundSprite.getLocalBounds().width;
    float scaleY = WINDOW_HEIGHT / backgroundSprite.getLocalBounds().height;
    backgroundSprite.setScale(scaleX, scaleY);

    //Настройка текста "Game Over"
    gameOverText.setFont(AssetManager::GetFont("retro.ttf"));
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(100);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(
        WINDOW_WIDTH / 2 - gameOverText.getLocalBounds().width / 2,
        WINDOW_HEIGHT / 2 - gameOverText.getLocalBounds().height / 2
    );

    //Установка иконки окна игрыы
    sf::Image icon = AssetManager::GetImage("icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());


    window.setFramerateLimit(60); //FPS
    //Шрифт очков
    scoreText.setFont(AssetManager::GetFont("retro.ttf"));
    scoreText.setCharacterSize(30);
    scoreText.setPosition(20, 20);
    
    //Загрузка уровня и звуковых эффектов
    loadLevel();
    hitPaddleSound.setBuffer(AssetManager::GetSoundBuffer("paddle_hit.wav"));
    hitBlockSound.setBuffer(AssetManager::GetSoundBuffer("block_hit.wav"));
    loseBallSound.setBuffer(AssetManager::GetSoundBuffer("lose_ball.wav"));

}

void GameEngine::loadLevel() {
    blocks.clear();
    //Рандомно расставляем блоки(50% зел, 30% жел, 20% красн)
    std::mt19937 gen(std::random_device{}());
    std::discrete_distribution<> dist{ 50, 30, 20 };

    const int cols = 10; // 10 блоков в ряду
    const int rows = 5;  // 5 рядов
    const float blockWidth = 90.f;
    const float blockHeight = 40.f;
    const float marginX = (WINDOW_WIDTH - (cols * blockWidth + (cols - 1) * 30)) / 2; // Было 10
    const float marginY = 60.f;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            Block::Type type;
            switch (dist(gen)) {
            case 0: type = Block::Type::Green; break;
            case 1: type = Block::Type::Yellow; break;
            case 2: type = Block::Type::Red; break;
            default: type = Block::Type::Green;
            }
            float posX = marginX + x * (blockWidth + 30); // Было +10
            float posY = marginY + y * (blockHeight + 30); // Было +15
            blocks.emplace_back(posX, posY, type);
        }
    }
}

void GameEngine::run() {
    sf::Clock clock; //Таймер для измерения времени между кадрами
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();//Вычисляем прошедшее в секундах время между кадрами)
        handleInput();
        update(dt); //Обновляем игровое состояние
        render(); //Отрисовываем кадр
    }
}

void GameEngine::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed ||
            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
            window.close();
        }
    }
}

//Обновление игрового состояния
void GameEngine::update(float dt) {
    if (gameOver) return; //Игра остановлена

    paddle.update(dt, WINDOW_WIDTH); //Обновляем позицию платформы
    ball.update(dt); //Обновляем позицию мяча
    checkCollisions(); //Проверяем столкновения

    //Обновление текста счета и жизней
    std::stringstream ss;
    ss << "Score: " << score << " |  Lives: " << lives;
    scoreText.setString(ss.str());
}


void GameEngine::checkCollisions() {
    Physics::checkPaddleCollision(ball, paddle, hitPaddleSound);
    Physics::checkBlockCollisions(ball, blocks, score, hitBlockSound);
    Physics::checkWallCollisions(ball, window);
    Physics::checkDeathZone(ball, lives, gameOver, window, loseBallSound);
}
//Отрисовка игрового состояния
void GameEngine::render() {
    window.clear(); //

    window.draw(backgroundSprite); //Рисуем фон

    if (gameOver) {
        window.draw(gameOverText); // выводим "Game Over"
    }
    else {
        // Рисуем все игровые объекты
        for (auto& block : blocks) block.draw(window); //Блоки
        paddle.draw(window); //Платформу
        ball.draw(window); //Мяч
        window.draw(scoreText); //Текст счета
    }

    window.display(); //Отображаем нарисованное
}