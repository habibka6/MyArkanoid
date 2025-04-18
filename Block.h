#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"

//Класс блоков(кирпичиков) для игры
class Block {
public:
    enum class Type { Green, Yellow, Red }; //Типы блоков
    Block(float x, float y, Type type); //Конструктор
    void draw(sf::RenderWindow& window); //Отрисовка блока
    sf::FloatRect getBounds() const; //Получение границ блока
    bool isDestroyed() const; //Проверка, уничтожен ли блок
    bool hit(); //Обработка попадания по блоку
    int getPoints() const; //Получение очков за блок

private:
    void updateColor();//Обновление цвета блока в зависимости от HP
    sf::RectangleShape shape;
    Type blockType;
    int maxHealth; //HP блока в начале уровня
    int currentHealth; //HP блока в настоящем времени
    int points; //Очки за блок
    bool destroyed; //Флаг разрушения
};