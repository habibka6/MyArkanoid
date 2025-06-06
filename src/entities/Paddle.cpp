#include "Paddle.h"
#include <algorithm>

namespace Arkanoid {

    Paddle::Paddle(float startX, float startY)
        : velocity(0.0f, 0.0f),
        active(true),
        speed(Config::Paddle::BASE_SPEED) {
        setupSprite();
        originalScale = sprite.getScale();
        setPosition({ startX, startY });
    }

    void Paddle::setupSprite() {
        sprite.setTexture(AssetManager::getInstance().getTexture("paddle.png"));
        sprite.setScale(2.0f, 2.0f);
    }

    void Paddle::update(float deltaTime) {
        if (!active) return;

        sf::Vector2f prevPos = getPosition();
        updateVelocity(prevPos, deltaTime);
    }

    void Paddle::draw(sf::RenderWindow& window) const {
        if (active) {
            window.draw(sprite);
        }
    }

    sf::Vector2f Paddle::getPosition() const {
        return sprite.getPosition();
    }

    sf::FloatRect Paddle::getBounds() const {
        return sprite.getGlobalBounds();
    }

    void Paddle::setPosition(const sf::Vector2f& position) {
        sprite.setPosition(position);
    }

    EntityType Paddle::getType() const {
        return EntityType::Paddle;
    }

    bool Paddle::isActive() const {
        return active;
    }

    void Paddle::setActive(bool active) {
        this->active = active;
    }

    sf::Vector2f Paddle::getVelocity() const {
        return velocity;
    }

    void Paddle::setVelocity(const sf::Vector2f& velocity) {
        this->velocity = velocity;
    }

    void Paddle::move(const sf::Vector2f& offset) {
        sprite.move(offset);
    }

    sf::FloatRect Paddle::getGlobalBounds() const {
        return sprite.getGlobalBounds();
    }

    sf::Vector2f Paddle::getSize() const {
        return {
            sprite.getGlobalBounds().width,
            sprite.getGlobalBounds().height
        };
    }

    float Paddle::getSpeed() const {
        return speed;
    }

    const sf::Sprite& Paddle::getSprite() const {
        return sprite;
    }
    sf::Sprite& Paddle::getSprite() {
        return sprite;
    }
    sf::Vector2f Paddle::getOriginalScale() {
        return originalScale;
    }

    void Paddle::resetToBaseScale() { 
        sprite.setScale(originalScale);
    }

    void Paddle::constrainToWindow(float windowWidth) {
        float x = std::clamp(
            sprite.getPosition().x,
            0.0f,
            windowWidth - sprite.getGlobalBounds().width
        );
        sprite.setPosition(x, sprite.getPosition().y);
    }

    void Paddle::updateVelocity(const sf::Vector2f& prevPos, float deltaTime) {
        if (deltaTime > 0) {
            velocity = (getPosition() - prevPos) / deltaTime;
        }
    }

} // namespace Arkanoid
