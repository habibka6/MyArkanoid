#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"

// Класс платформы для игры
class Paddle {
public:
    Paddle(float startX, float startY); //Конструктор
    void update(float dt, float windowWidth); //Обновление позиции 
    void draw(sf::RenderWindow& window); //Отрисовка
    //Геттеры
    sf::FloatRect getGlobalBounds() const; // Получение границ 
    sf::Vector2f getPosition() const; // Получение позиции платформы
    sf::Vector2f getSize() const; // Получение размера платформы

private:
    sf::Sprite sprite; 
    const float speed = 600.0f; 
};