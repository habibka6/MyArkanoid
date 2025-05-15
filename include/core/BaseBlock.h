#pragma once
#include <SFML/Graphics.hpp>

class BaseBlock {
public:
    virtual ~BaseBlock() = default;

    //ќтрисовка блока в окне
    virtual void draw(sf::RenderWindow& wnd) = 0;

    // ѕолучение границ блока (дл€ коллизий)
    virtual sf::FloatRect getBounds() const = 0;

    // ќбработка удара по блоку. ¬озвращает true, если блок уничтожен
    virtual bool hit() = 0;

    // ѕроверка, уничтожен ли блок
    virtual bool isDestroyed() const = 0;

    // ѕолучение очков за уничтожение блока
    virtual int getPoints() const = 0;

    // явл€етс€ ли блок каменным (неразрушимым)
    virtual bool IsRock() = 0;
};
