#include "GameEngine.h"

GameEngine::GameEngine()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Arkanoid"),
    paddle(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT - 50), //Центрирование платформы
    ball(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100) 
{    


    //Загрузка фона и масштабирование под размер окна
    backgroundSprite.setTexture(AssetManager::getInstance().GetTexture("background.png"));
    
    float scaleX = WINDOW_WIDTH / backgroundSprite.getLocalBounds().width;
    float scaleY = WINDOW_HEIGHT / backgroundSprite.getLocalBounds().height;
    backgroundSprite.setScale(scaleX, scaleY);

    //Настройка текста "Game Over"
    gameOverText.setFont(AssetManager::getInstance().GetFont("retro.ttf"));
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(100);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(
        WINDOW_WIDTH / 2 - gameOverText.getLocalBounds().width / 2,
        WINDOW_HEIGHT / 2 - gameOverText.getLocalBounds().height / 2
    );

    //Установка иконки окна игрыы
    sf::Image icon = AssetManager::getInstance().GetImage("icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());


    window.setFramerateLimit(70); //FPS
    //Шрифт очков
    scoreText.setFont(AssetManager::getInstance().GetFont("retro.ttf"));
    scoreText.setCharacterSize(30);
    scoreText.setPosition(20, 20);
    
    //Загрузка уровня и звуковых эффектов
    loadLevel();
    hitPaddleSound.setBuffer(AssetManager::getInstance().GetSoundBuffer("paddle_hit.wav"));
    hitBlockSound.setBuffer(AssetManager::getInstance().GetSoundBuffer("block_hit.wav"));
    hitRockSound.setBuffer(AssetManager::getInstance().GetSoundBuffer("rock_hit.wav"));
    loseBallSound.setBuffer(AssetManager::getInstance().GetSoundBuffer("lose_ball.wav"));

}

void GameEngine::loadLevel() {
    blocks.clear();

    // 1) Параметры поля
    const int cols = 10;
    const int rows = 5;
    const float blockW = 90.f;
    const float blockH = 40.f;
    const float spacing = 30.f;
    const float marginX = (WINDOW_WIDTH - (cols * blockW + (cols - 1) * spacing)) / 2;
    const float marginY = 60.f;

    // 2) Сколько нужно камней и все индексы
    const int rockCount = 5;
    std::vector<int> allIdx(cols * rows);
    std::iota(allIdx.begin(), allIdx.end(), 0);

    // 3) Перемешиваем
    std::mt19937 gen(std::random_device{}());
    std::shuffle(allIdx.begin(), allIdx.end(), gen);

    // 4) Отбираем индексы для камней без соседей
    std::unordered_set<int> rockSet;
    auto hasNeighbor = [&](int idx) {
        int x = idx % cols, y = idx / cols;
        // проверяем 4 соседей
        if (x > 0 && rockSet.count(idx - 1))      return true;
        if (x < cols - 1 && rockSet.count(idx + 1))      return true;
        if (y > 0 && rockSet.count(idx - cols))   return true;
        if (y < rows - 1 && rockSet.count(idx + cols))   return true;
        return false;
        };

    for (int idx : allIdx) {
        if ((int)rockSet.size() >= rockCount)
            break;
        if (!hasNeighbor(idx)) {
            rockSet.insert(idx);
        }
    }
    // если не набрали нужное количество (крайне маловероятно), 
    // можно просто докинуть из оставшихся:
    for (int idx : allIdx) {
        if ((int)rockSet.size() >= rockCount)
            break;
        rockSet.insert(idx);
    }

    // 5) Дистрибуция для цветных блоков
    std::discrete_distribution<> colorDist{ 45, 25, 20 };

    // 6) Строим вектор unique_ptr<BlockBase>
    for (int idx = 0; idx < cols * rows; ++idx) {
        int y = idx / cols;
        int x = idx % cols;
        float posX = marginX + x * (blockW + spacing);
        float posY = marginY + y * (blockH + spacing);

        if (rockSet.count(idx)) {
            // каменный блок
            blocks.emplace_back(std::make_unique<Rock>(posX, posY));
        }
        else {
            // обычный цветной
            int d = colorDist(gen);
            Block::Type type = (d == 0 ? Block::Type::Green
                : d == 1 ? Block::Type::Yellow
                : Block::Type::Red);
            blocks.emplace_back(std::make_unique<Block>(posX, posY, type));
        }
    }
}

void GameEngine::resetBallPosition()
{
    // Центр платформы
    float px = paddle.getPosition().x + paddle.getSize().x / 2;
    float py = paddle.getPosition().y;
    // Ставим мяч чуть выше
    ball.getSprite().setPosition(px, py - ball.getBounds().height);
}

void GameEngine::launchBall()
{
    // Случайный угол от -60° до +60°
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(-20.f, 20.f);
    float angleDeg = dist(gen) * 3.1415926f / 180.f;

    float speed = ball.getSpeed();
    float vx = std::sin(angleDeg) * speed;
    float vy = -std::cos(angleDeg) * speed;

    ball.setVelocity(vx, vy);
    waitingLaunch = false;
}

void GameEngine::run()
{
    sf::Clock clock;
    const float fixedStep = 1.f / 120.f;   // шаг физики
    const int   maxSubSteps = 5;            // не более 5 подшагов за кадр

    float accumulator = 0.f;

    while (window.isOpen())
    {
        // 1) Считаем dt и ограничиваем броски
        float dt = clock.restart().asSeconds();
        if (dt > 0.25f)               // защита от долгих фризов
            dt = 0.25f;

        accumulator += dt;

        // 2) Ввод обрабатываем один раз за кадр
        handleInput();

        // 3) Подшаги физики
        int subSteps = 0;
        while (accumulator >= fixedStep && subSteps < maxSubSteps)
        {
            ball.savePrevPosition();
            update(fixedStep);
            accumulator -= fixedStep;
            subSteps++;
        }

        // 4) Интерполяция для плавной отрисовки
        float alpha = accumulator / fixedStep;
        render(alpha);
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
void GameEngine::update(float dt)
{
    if (gameOver) return;

    paddle.update(dt, WINDOW_WIDTH);

    if (waitingLaunch) {
        // Привязываем шар к платформе
        resetBallPosition();

        // Если нажали Space — запускаем
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            launchBall();
        }
    }
    else {
        // Обычное поведение
        ball.update(dt);
        checkCollisions(dt);

        // Проверка смерти (там выставится waitingLaunch=true)
        // и мяч прилипнет к платформе снова
    }
    // Обновляем текст
    std::stringstream ss;
    ss << "Score: " << score << " |  Lives: " << lives;
    scoreText.setString(ss.str());
}



void GameEngine::checkCollisions(float dt) {
    Physics::checkPaddleCollision(ball, paddle, hitPaddleSound);
    Physics::checkBlockCollisions(ball, blocks, score,dt, hitBlockSound,hitRockSound);
    Physics::checkWallCollisions(ball, window);

    // Проверяем зону смерти
    int oldLives = lives;
    Physics::checkDeathZone(ball, lives, gameOver, window, loseBallSound);
    if (lives < oldLives && !gameOver) {
        // потеряли мяч – возвращаемся в режим ожидания
        waitingLaunch = true;
    }
}
//Отрисовка игрового состояния
void GameEngine::render(float alpha) {
    window.clear();
    window.draw(backgroundSprite);

    if (gameOver) {
        window.draw(gameOverText);
    } else {
        // Отрисовываем все блоки, полиморфно
        for (auto& blk : blocks)
            blk->draw(window);

        paddle.draw(window);
        ball.draw(window, alpha);
        window.draw(scoreText);
    }

    window.display();
}
