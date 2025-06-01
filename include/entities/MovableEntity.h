#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
namespace Arkanoid {

    class MovableEntity : public Entity {
    public:
        virtual ~MovableEntity() = default;

        virtual sf::Vector2f getVelocity() const = 0;
        virtual void setVelocity(const sf::Vector2f& velocity) = 0;
        virtual void move(const sf::Vector2f& offset) = 0;

    protected:
        MovableEntity() = default;
    };

} // namespace Arkanoid
