#include "GameEngine.h"
#include <sstream>

GameEngine::GameEngine()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Arkanoid"),
    paddle(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT - 50), //������������� ���������
    ball(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100) 
{    

    //�������� ���� � ��������������� ��� ������ ����
    backgroundSprite.setTexture(AssetManager::GetTexture("background.png"));
    
    float scaleX = WINDOW_WIDTH / backgroundSprite.getLocalBounds().width;
    float scaleY = WINDOW_HEIGHT / backgroundSprite.getLocalBounds().height;
    backgroundSprite.setScale(scaleX, scaleY);

    //��������� ������ "Game Over"
    gameOverText.setFont(AssetManager::GetFont("retro.ttf"));
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(100);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(
        WINDOW_WIDTH / 2 - gameOverText.getLocalBounds().width / 2,
        WINDOW_HEIGHT / 2 - gameOverText.getLocalBounds().height / 2
    );

    //��������� ������ ���� �����
    sf::Image icon = AssetManager::GetImage("icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());


    window.setFramerateLimit(60); //FPS
    //����� �����
    scoreText.setFont(AssetManager::GetFont("retro.ttf"));
    scoreText.setCharacterSize(30);
    scoreText.setPosition(20, 20);
    
    //�������� ������ � �������� ��������
    loadLevel();
    hitPaddleSound.setBuffer(AssetManager::GetSoundBuffer("paddle_hit.wav"));
    hitBlockSound.setBuffer(AssetManager::GetSoundBuffer("block_hit.wav"));
    loseBallSound.setBuffer(AssetManager::GetSoundBuffer("lose_ball.wav"));

}

void GameEngine::loadLevel() {
    blocks.clear();
    //�������� ����������� �����(50% ���, 30% ���, 20% �����)
    std::mt19937 gen(std::random_device{}());
    std::discrete_distribution<> dist{ 50, 30, 20 };

    const int cols = 10; // 10 ������ � ����
    const int rows = 5;  // 5 �����
    const float blockWidth = 90.f;
    const float blockHeight = 40.f;
    const float marginX = (WINDOW_WIDTH - (cols * blockWidth + (cols - 1) * 10)) / 2; //������������� ������
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
            float posX = marginX + x * (blockWidth + 10);
            float posY = marginY + y * (blockHeight + 15);
            blocks.emplace_back(posX, posY, type);
        }
    }
}

void GameEngine::run() {
    sf::Clock clock; //������ ��� ��������� ������� ����� �������
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();//��������� ��������� � �������� ����� ����� �������)
        handleInput();
        update(dt); //��������� ������� ���������
        render(); //������������ ����
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

//���������� �������� ���������
void GameEngine::update(float dt) {
    if (gameOver) return; //���� �����������

    paddle.update(dt, WINDOW_WIDTH); //��������� ������� ���������
    ball.update(dt); //��������� ������� ����
    checkCollisions(); //��������� ������������

    //���������� ������ ����� � ������
    std::stringstream ss;
    ss << "Score: " << score << " |  Lives: " << lives;
    scoreText.setString(ss.str());
}


//�������� ���� ������������ � ����
void GameEngine::checkCollisions() {
    //������������ ���� � ����������
    if (ball.getBounds().intersects(paddle.getGlobalBounds())) {
        // ��������� ����� ����� ������������ ������ ���������
        float paddleCenterX = paddle.getPosition().x + paddle.getSize().x / 2;
        float hitPos = (ball.getPosition().x - paddleCenterX) / (paddle.getSize().x / 2);
        hitPos = std::clamp(hitPos, -1.0f, 1.0f); // ����������� �������� (�� -1 �� 1)
        //��������� ���� ������� � ����������� �� ����� �����
        float angle = hitPos * 60.0f * 3.14159f / 180.0f;
        ball.setVelocity(500.f * sin(angle), -500.f * cos(angle));
        hitPaddleSound.play(); 
    }

    //������������ ���� � �������
    for (auto it = blocks.begin(); it != blocks.end();) {
        if (!(it->isDestroyed()) && ball.getBounds().intersects(it->getBounds())) {
            sf::FloatRect blockBounds = it->getBounds();
            float overlapLeft = ball.getBounds().left + ball.getBounds().width - blockBounds.left;
            float overlapRight = blockBounds.left + blockBounds.width - ball.getBounds().left;
            float overlapTop = ball.getBounds().top + ball.getBounds().height - blockBounds.top;
            float overlapBottom = blockBounds.top + blockBounds.height - ball.getBounds().top;
            hitBlockSound.play();
    
            //��������� "�������" ����������� �� ���� ������(������ ��� ������/���� �� ������ ���������)
            overlapLeft = std::max(0.0f, overlapLeft);
            overlapRight = std::max(0.0f, overlapRight);
            overlapTop = std::max(0.0f, overlapTop);
            overlapBottom = std::max(0.0f, overlapBottom);

            //������� ����������� ���������� (������� ������������)
            float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

            //���������� ������� �������
            sf::Vector2f normal;
            if (minOverlap == overlapLeft) normal = { -1, 0 };
            else if (minOverlap == overlapRight) normal = { 1, 0 };
            else if (minOverlap == overlapTop) normal = { 0, -1 };
            else normal = { 0, 1 };

            //��������� ���� �� �������
            ball.reflect(normal);


            if (it->hit()) {
                score += it->getPoints();
                it = blocks.erase(it);
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }

    //������������ �� �������
    sf::Vector2f ballPos = ball.getPosition();
    float ballRadius = ball.getBounds().width / 2;

    if (ballPos.x - ballRadius <= 0) {
        ball.reflect({ 1, 0 }); //��������� �� ����� �����
    }
    else if (ballPos.x + ballRadius >= WINDOW_WIDTH) {
        ball.reflect({ -1, 0 }); //��������� �� ������ �����
    }
    if (ballPos.y - ballRadius <= 0) {
        ball.reflect({ 0, 1 });
    }

    //������ ����
    if (ballPos.y + ballRadius >= WINDOW_HEIGHT) {
        lives--; //��������� �����
        if (lives <= 0) {
            gameOver = true;
        }
        else {
            ball.reset(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 150);
        }
        loseBallSound.play();
    }
}

//��������� �������� ���������
void GameEngine::render() {
    window.clear(); //

    window.draw(backgroundSprite); //������ ���

    if (gameOver) {
        window.draw(gameOverText); // ������� "Game Over"
    }
    else {
        // ������ ��� ������� �������
        for (auto& block : blocks) block.draw(window); //�����
        paddle.draw(window); //���������
        ball.draw(window); //���
        window.draw(scoreText); //����� �����
    }

    window.display(); //���������� ������������
}