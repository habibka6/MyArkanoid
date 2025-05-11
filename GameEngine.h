#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "Paddle.h"
#include "Ball.h"
#include "Block.h"
#include "AssetManager.h"
#include "Physics.h"
// Основной класс игры
class GameEngine {
public:
    GameEngine(); //Конструктор
    void run(); //Запуск игрового цикла

private:
    const unsigned int WINDOW_WIDTH = 1280; 
    const unsigned int WINDOW_HEIGHT = 720;

    sf::RenderWindow window; //Окно игры
    Paddle paddle; //Платформа
    Ball ball; //Мяч
    std::vector<Block> blocks; //Последовательность блоков

    sf::Text scoreText; //Счёт очков
    sf::Sprite backgroundSprite; //Фон

    sf::Sound hitPaddleSound; //Звук удара о платформу
    sf::Sound hitBlockSound; //Звук удара о блок
    sf::Sound loseBallSound; //Звук потери мяча(минус HP)

    int score = 0; //Счёт
    int lives = 3; //Количество жизней
    bool gameOver = false; //Флаг окончания игры
    sf::Text gameOverText; //Текст "Game Over"

    void loadLevel(); //Загрузка уровня
    void handleInput(); //Обработка ввода
    void update(float dt); //Обновление состояния игры
    void render(); //Отрисовка игры
    void checkCollisions(); //Проверка столкновений
};