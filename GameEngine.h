#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "Paddle.h"
#include "Ball.h"
#include "Block.h"
#include "AssetManager.h"
#include "Physics.h"
// �������� ����� ����
class GameEngine {
public:
    GameEngine(); //�����������
    void run(); //������ �������� �����

private:
    const unsigned int WINDOW_WIDTH = 1280; 
    const unsigned int WINDOW_HEIGHT = 720;

    sf::RenderWindow window; //���� ����
    Paddle paddle; //���������
    Ball ball; //���
    std::vector<Block> blocks; //������������������ ������

    sf::Text scoreText; //���� �����
    sf::Sprite backgroundSprite; //���

    sf::Sound hitPaddleSound; //���� ����� � ���������
    sf::Sound hitBlockSound; //���� ����� � ����
    sf::Sound loseBallSound; //���� ������ ����(����� HP)

    int score = 0; //����
    int lives = 3; //���������� ������
    bool gameOver = false; //���� ��������� ����
    sf::Text gameOverText; //����� "Game Over"

    void loadLevel(); //�������� ������
    void handleInput(); //��������� �����
    void update(float dt); //���������� ��������� ����
    void render(); //��������� ����
    void checkCollisions(); //�������� ������������
};