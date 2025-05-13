#include "Block.h"
Block::Block(float x, float y, Type type)
    : blockType(type), destroyed(false){
    shape.setSize(sf::Vector2f(100, 40));
    shape.setPosition(x, y);

    //Установка параметров в зависимости от типа блока

    switch (blockType) {
    case Type::Green:
        maxHealth = currentHealth = 1;
        points = 10;
        shape.setFillColor(sf::Color::Green);
        break;
    case Type::Yellow:
        maxHealth = currentHealth = 2;
        points = 20;
        shape.setFillColor(sf::Color(255, 255, 0));
        break;
    case Type::Red:
        maxHealth = currentHealth = 3;
        points = 30;
        shape.setFillColor(sf::Color(255, 0, 0));
        break;
    }

    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Black);
}

//Обработка попадания по блоку
bool Block::hit ()  {
    currentHealth--;
    updateColor();
    if (currentHealth <= 0) {
        destroyed = true;
        return true;
    }
    return false;
}

//Визуализация степени разрушенности блока(Обновление цвета на более тусклый в первой версии игры)
void Block::updateColor() {
    float ratio = static_cast<float>(currentHealth) / maxHealth;
    switch (blockType) {
    case Type::Green: break;
    case Type::Yellow:
        shape.setFillColor(sf::Color(205, 164, 52));
        break;
    case Type::Red:
        shape.setFillColor(sf::Color(255 * ratio, 0, 0));
        break;
    }
}

// Геттеры
bool Block::isDestroyed() const { return destroyed; }
int Block::getPoints() const { return points; }
sf::FloatRect Block::getBounds() const { return shape.getGlobalBounds(); }
void Block::draw(sf::RenderWindow& window) {
    if (!destroyed) window.draw(shape);
}





