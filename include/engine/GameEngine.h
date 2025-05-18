#pragma once
#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "Block.h"
#include "AssetManager.h"
#include "BlockCollision.h"
#include "WallCollision.h"
#include "PaddleCollision.h"
#include "BaseBlock.h"
#include "Rock.h"
#include "Config.h"
#include <sstream>
#include <unordered_set>
#include <numeric>
#include <vector>
#include <random>
// �������� ����� ����
class GameEngine {
public:
    GameEngine(); //�����������
    void run(); //������ �������� �����

private:

    sf::RenderWindow window; //���� ����
    Paddle paddle; //���������
    Ball ball; //���
    std::vector<std::unique_ptr<BaseBlock>> blocks; //������������������ ������

    sf::Text scoreText; //���� �����
    sf::Sprite backgroundSprite; //���

    sf::Sound hitPaddleSound; //���� ����� � ���������
    sf::Sound hitBlockSound; //���� ����� � ����
    sf::Sound hitRockSound; //���� ����� � ������
    sf::Sound loseBallSound; //���� ������ ����(����� HP)

    int score = 0; //����
    int lives = 3; //���������� ������
    bool gameOver = false; //���� ��������� ����
    bool waitingLaunch = true;  // ��� ������� � ���������, ���� ������
    sf::Text gameOverText; //����� "Game Over"

    void loadLevel(); //�������� ������
    void launchBall();         // ����� ����� ��� ������ ����
    void resetBallPosition();  // ��������� ��� � ���������
    void handleInput(); //��������� �����
    void update(float dt); //���������� ��������� ����
    void render(float alpha); //��������� ����
    void checkCollisions(float dt); //�������� ������������
};