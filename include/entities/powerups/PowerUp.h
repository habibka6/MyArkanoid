#pragma once
#include "MovableEntity.h"
#include "PowerUpEffect.h"
#include "Config.h"
#include <SFML/Graphics.hpp>
#include <memory>

namespace Arkanoid {

    class PowerUp : public MovableEntity {
    public:
        PowerUp(float x, float y);

        virtual void setupAppearance() = 0;
        virtual PowerUpType getPowerUpType() const = 0;
        virtual std::unique_ptr<PowerUpEffect> createEffect() = 0;


        void update(float deltaTime) override;
        void draw(sf::RenderWindow& window) const override;
        sf::Vector2f getPosition() const override;
        void setPosition(const sf::Vector2f& pos) override;
        sf::FloatRect getBounds() const override;
        EntityType getType() const override;
        bool isActive() const override;
        void setActive(bool a) override;
        sf::Vector2f getVelocity() const override;
        void setVelocity(const sf::Vector2f& v) override;
        void move(const sf::Vector2f& offset) override;

    protected:
        sf::RectangleShape shape;
        bool active;
        float fallSpeed;
    };

} // namespace Arkanoid
