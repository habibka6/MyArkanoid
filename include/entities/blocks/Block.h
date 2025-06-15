#pragma once
#include "BaseBlock.h"
#include "Config.h"
namespace Arkanoid {

    class Block : public BaseBlock {
    public:
        enum class Type {
            Green,
            Yellow,
            Red
        };

        Block(float x, float y, Type type);
        ~Block() = default;

        // Entity interface
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
        sf::Color getColor() const override;

    private:
        sf::RectangleShape shape;
        Type blockType;
        int maxHealth;
        int currentHealth;
        bool destroyed;
        bool active;
        int pointValue;

        void setupBlock();
        void updateAppearance();
        sf::Color getColorForType(Type type, int health, int maxHealth);
    };

} 
