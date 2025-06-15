#pragma once
#include "MovableEntity.h"
#include "AssetManager.h"
#include "Config.h"

namespace Arkanoid {

    class Paddle : public MovableEntity {
    public:
        Paddle(float startX, float startY);
        ~Paddle() = default;

        void update(float deltaTime) override;
        void draw(sf::RenderWindow& window) const override;
        sf::Vector2f getPosition() const override;
        sf::FloatRect getBounds() const override;
        void setPosition(const sf::Vector2f& position) override;
        EntityType getType() const override;
        bool isActive() const override;
        void setActive(bool active) override;

        sf::Vector2f getVelocity() const override;
        void setVelocity(const sf::Vector2f& velocity) override;
        void move(const sf::Vector2f& offset) override;

        sf::FloatRect getGlobalBounds() const;
        sf::Vector2f getSize() const;
        float getSpeed() const;
        sf::Sprite& getSprite();
        const sf::Sprite& getSprite() const;
        sf::Vector2f getOriginalScale();
        void constrainToWindow(float windowWidth);
        void resetToBaseScale();
    private:
        sf::Vector2f originalScale;
        sf::Sprite sprite;
        sf::Vector2f velocity;
        bool active;
        const float speed;

        void setupSprite();
        void updateVelocity(const sf::Vector2f& prevPos, float deltaTime);
    };

} 
