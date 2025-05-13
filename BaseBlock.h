#pragma once
#include <SFML/Graphics.hpp>

class BaseBlock
{
public:
    virtual ~BaseBlock() = default;

    virtual void draw(sf::RenderWindow& wnd) = 0;
    virtual sf::FloatRect getBounds() const = 0;
    virtual bool hit() = 0;  
    virtual bool isDestroyed() const = 0;
    virtual int  getPoints() const = 0;
    virtual bool IsRock() = 0;
};
