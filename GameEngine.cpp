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
    const float marginX = (WINDOW_WIDTH - (cols * blockWidth + (cols - 1) * 30)) / 2; // ���� 10
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
            float posX = marginX + x * (blockWidth + 30); // ���� +10
            float posY = marginY + y * (blockHeight + 30); // ���� +15
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


void GameEngine::checkCollisions() {
    Physics::checkPaddleCollision(ball, paddle, hitPaddleSound);
    Physics::checkBlockCollisions(ball, blocks, score, hitBlockSound);
    Physics::checkWallCollisions(ball, window);
    Physics::checkDeathZone(ball, lives, gameOver, window, loseBallSound);
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