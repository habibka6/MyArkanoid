#pragma once
#include "MovableEntity.h"
#include "AssetManager.h"
#include "Config.h"

namespace Arkanoid {

    class Ball : public MovableEntity {
    public:
        Ball(float startX, float startY);
        ~Ball() = default;

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

        // Ball specific methods
        void reset(float x, float y);
        void reflect(const sf::Vector2f& normal);
        void increaseSpeedFactor(float value);
        void resetSpeedFactor();
        float getSpeedFactor() const;
        float getBaseSpeed() const;
        sf::Sprite& getSprite();
        void draw(sf::RenderWindow& window, float alpha) const;

    private:
        sf::Sprite sprite;
        sf::Vector2f velocity;
        sf::Vector2f previousPosition;
        float baseSpeed;
        float speedFactor;
        bool active;

        void savePreviousPosition();
        void normalizeVelocity();
        void setupSprite();
    };

} // namespace Arkanoid
