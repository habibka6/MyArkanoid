#pragma once
#include <SFML/Graphics.hpp>
#include "BaseBlock.h"

class Block : public BaseBlock
{
public:
    enum class Type { Green, Yellow, Red };

    Block(float x, float y, Type t);

    // Переопределяем виртуальные
    void draw(sf::RenderWindow& wnd) override;
    sf::FloatRect getBounds() const override;
    bool hit() override;
    bool isDestroyed() const override;
    int  getPoints() const override;
    bool IsRock() override { return false; }

private:
    void updateColor();

    sf::RectangleShape shape;
    Type blockType;
    int  maxHealth, currentHealth;
    int  points;
    bool destroyed;
};