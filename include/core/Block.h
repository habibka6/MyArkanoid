#pragma once
#include <SFML/Graphics.hpp>
#include "BaseBlock.h"

class Block : public BaseBlock {
public:
    // Типы блоков (цвет и прочность)
    enum class Type { Green, Yellow, Red };

    Block(float x, float y, Type type);

    // Реализация методов BaseBlock
    void draw(sf::RenderWindow& wnd) override;
    sf::FloatRect getBounds() const override;
    bool hit() override;
    bool isDestroyed() const override;
    int  getPoints() const override;
    bool IsRock() override { return false; }

private:
    // Обновление цвета блока в зависимости от здоровья
    void updateColor();

    sf::RectangleShape shape; // Графическое представление
    Type blockType; // Тип блока
    int  maxHealth; // Максимальное здоровье
    int  currentHealth; // Текущее здоровье
    int  points; // Очки за уничтожение
    bool destroyed; // Флаг уничтожения
};