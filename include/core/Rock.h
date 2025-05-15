#pragma once
#include <SFML/Graphics.hpp>
#include "BaseBlock.h"

class Rock : public BaseBlock
{
public:
    Rock(float x, float y);
    // Реализация методов BaseBlock
    void draw(sf::RenderWindow& wnd) override;
    sf::FloatRect getBounds() const override;
    bool hit() override;   // всегда false
    bool isDestroyed() const override;   // никогда
    int  getPoints() const override;   // 0
    bool IsRock() override { return true; }
private:
    sf::RectangleShape shape; // Графическое представление
};
