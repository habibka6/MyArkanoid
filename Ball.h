#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"

// ласс м€ча(шарика) дл€ игры
class Ball {
public:
    Ball(float startX, float startY); // онструтктор
    void update(float dt); //ќбновление позиции м€ча
    void draw(sf::RenderWindow& window); //ќтрисовка
    void reflect(const sf::Vector2f& normal); //ќтражение м€ча от поверхности
    void reset(float x, float y); //—брос м€ча в начальное положение
    void correctPosition(const sf::FloatRect& paddleBounds, float dt); // оррекци€ позиции м€ча

    //√еттеры
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
    float getSpeed() const;
    void setVelocity(float x, float y); //”становка скорости

private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    const float baseSpeed = 600.0f; 
};