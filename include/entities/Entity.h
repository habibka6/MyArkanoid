#pragma once
#include <SFML/Graphics.hpp>

namespace Arkanoid {

    enum class EntityType {
        Ball,
        Paddle,
        Block,
        Rock,
        PowerUp
    };

    class Entity {
    public:
        virtual ~Entity() = default;

        virtual void update(float deltaTime) = 0;
        virtual void draw(sf::RenderWindow& window) const = 0;
        virtual sf::Vector2f getPosition() const = 0;
        virtual sf::FloatRect getBounds() const = 0;
        virtual void setPosition(const sf::Vector2f& position) = 0;
        virtual EntityType getType() const = 0;
        virtual bool isActive() const = 0;
        virtual void setActive(bool active) = 0;

    protected:
        Entity() = default;
    };

} 
