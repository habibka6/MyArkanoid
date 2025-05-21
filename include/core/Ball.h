#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "Config.h"
//Класс мяча(шарика) для игры
class Ball {
public:
    Ball(float startX, float startY); //Конструтктор
    void update(float dt);//Обновление позиции мяча
    void draw(sf::RenderWindow& window,float alpha) const; //Отрисовка
    void reflect(const sf::Vector2f& normal); //Отражение мяча от поверхности
    void reset(float x, float y); //Сброс мяча в начальное положение
    void correctPosition(const sf::FloatRect& paddleBounds, float dt); //Коррекция позиции мяча
   
    //Геттеры
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
    sf::Sprite& getSprite() { return sprite; }

    
    float getSpeed() const;
    void setVelocity(float x, float y); //Установка скорости
    void savePrevPosition() {
        m_prevPosition = sprite.getPosition();
    }
    void resetSpeedFactor() { speedFactor = 1.0f; }
    void increaseSpeedFactor(float value);
    float getSpeedFactor() const { return speedFactor; }
private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    const float baseSpeed = BASE_BALL_SPEED;
    float speedFactor = 1.0f; // Множитель скорости
    sf::Vector2f m_prevPosition; // Предыдущая позиция для интерполяции
};