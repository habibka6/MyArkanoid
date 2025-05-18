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
// Основной класс игры
class GameEngine {
public:
    GameEngine(); //Конструктор
    void run(); //Запуск игрового цикла

private:

    sf::RenderWindow window; //Окно игры
    Paddle paddle; //Платформа
    Ball ball; //Мяч
    std::vector<std::unique_ptr<BaseBlock>> blocks; //Последовательность блоков

    sf::Text scoreText; //Счёт очков
    sf::Sprite backgroundSprite; //Фон

    sf::Sound hitPaddleSound; //Звук удара о платформу
    sf::Sound hitBlockSound; //Звук удара о блок
    sf::Sound hitRockSound; //Звук удара о камень
    sf::Sound loseBallSound; //Звук потери мяча(минус HP)

    int score = 0; //Счёт
    int lives = 3; //Количество жизней
    bool gameOver = false; //Флаг окончания игры
    bool waitingLaunch = true;  // мяч «прилип» к платформе, ждем пробел
    sf::Text gameOverText; //Текст "Game Over"

    void loadLevel(); //Загрузка уровня
    void launchBall();         // новый метод для старта мяча
    void resetBallPosition();  // прилипает мяч к платформе
    void handleInput(); //Обработка ввода
    void update(float dt); //Обновление состояния игры
    void render(float alpha); //Отрисовка игры
    void checkCollisions(float dt); //Проверка столкновений
};