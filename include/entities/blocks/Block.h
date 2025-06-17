#pragma once
#include "BaseBlock.h"
#include "Config.h"
namespace Arkanoid {

    class Block : public BaseBlock {
    public:

        // Вариации разрушаемых блоков
        enum class Type {
            Green,
            Yellow,
            Red
        };

        Block(float x, float y, Type type);
        ~Block() = default;

        void update(float deltaTime) override;
        void draw(sf::RenderWindow& window) const override;
        sf::Vector2f getPosition() const override;
        sf::FloatRect getBounds() const override;
        void setPosition(const sf::Vector2f& position) override;
        bool isActive() const override;
        void setActive(bool active) override;

 
        void hit() override;
        bool isDestroyed() const override;
        int getPoints() const override;
        BlockType getBlockType() const override;

    private:
        sf::RectangleShape shape;
        Type blockType;
        int maxHealth;
        int currentHealth;
        bool destroyed;
        bool active;
        int pointValue;

        void setupBlock(); // Инициализация блока
        void updateAppearance(); // Обновление внешнего вида (цвета) после удара мяча
        sf::Color getColorForType(Type type, int health, int maxHealth);
    };

} 
