#pragma once
#include "MovableEntity.h"
#include "PowerUpEffect.h"
#include "Config.h"
#include <memory>

namespace Arkanoid {

    class BasePowerUp : public MovableEntity {
    public:
        BasePowerUp(float x, float y, PowerUpType type);
        virtual ~BasePowerUp() = default;

        // Entity interface
        void update(float deltaTime) override;
        void draw(sf::RenderWindow& window) const override;
        sf::Vector2f getPosition() const override;
        sf::FloatRect getBounds() const override;
        void setPosition(const sf::Vector2f& position) override;
        EntityType getType() const override;
        bool isActive() const override;
        void setActive(bool active) override;

        // MovableEntity interface
        sf::Vector2f getVelocity() const override;
        void setVelocity(const sf::Vector2f& velocity) override;
        void move(const sf::Vector2f& offset) override;

        // PowerUp specific methods
        PowerUpType getPowerUpType() const;
        virtual std::unique_ptr<PowerUpEffect> createEffect() = 0;
        bool isOutOfBounds(float windowHeight) const;

    protected:
        virtual void setupAppearance() = 0;

        sf::RectangleShape shape;
        sf::Vector2f velocity;
        PowerUpType powerUpType;
        bool active;
        float fallSpeed;
    };

} // namespace Arkanoid
