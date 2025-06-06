#include "Ball.h"
#include <cmath>
#include <random>

namespace Arkanoid {

    Ball::Ball(float startX, float startY)
        : baseSpeed(Config::Ball::BASE_SPEED),
        speedFactor(1.0f),
        active(true),
        isOnPaddle(true)        
    {  
        setupSprite();
        reset(startX, startY);
    }

    void Ball::setupSprite() {
        sprite.setTexture(AssetManager::getInstance().getTexture("ball.png"));
        sprite.setScale(1.5f, 1.5f);
        sprite.setOrigin(
            sprite.getLocalBounds().width / 2,
            sprite.getLocalBounds().height / 2
        );
    }

    void Ball::update(float deltaTime) {
        if (!active) return;

        // Обновляем только если мяч НЕ на платформе
        if (!isOnPaddle) {
            savePreviousPosition();
            move(velocity * deltaTime);
        }
    }

    void Ball::reset(float paddleCenterX, float paddleY) {
        isOnPaddle = true;
        setPosition({ paddleCenterX, paddleY - getBounds().height });
        velocity = { 0.0f, 0.0f }; // Останавливаем мяч
        resetSpeedFactor();
    }


    void Ball::draw(sf::RenderWindow& window) const {
        if (active) {
            window.draw(sprite);
        }
    }

    void Ball::draw(sf::RenderWindow& window, float alpha) const {
        if (!active) return;

        sf::Sprite tempSprite = sprite;
        sf::Vector2f interpolatedPos = previousPosition * (1 - alpha) + sprite.getPosition() * alpha;
        tempSprite.setPosition(interpolatedPos);
        window.draw(tempSprite);
    }

    sf::Vector2f Ball::getPosition() const {
        return sprite.getPosition();
    }

    sf::FloatRect Ball::getBounds() const {
        return sprite.getGlobalBounds();
    }

    void Ball::setPosition(const sf::Vector2f& position) {
        sprite.setPosition(position);
    }

    EntityType Ball::getType() const {
        return EntityType::Ball;
    }

    bool Ball::isActive() const {
        return active;
    }

    void Ball::setActive(bool active) {
        this->active = active;
    }

    sf::Vector2f Ball::getVelocity() const {
        return velocity;
    }

    void Ball::setVelocity(const sf::Vector2f& velocity) {
        this->velocity = velocity;
        normalizeVelocity();
    }

    void Ball::move(const sf::Vector2f& offset) {
        sprite.move(offset);
    }


    void Ball::reflect(const sf::Vector2f& normal) {
        float dot = velocity.x * normal.x + velocity.y * normal.y;
        velocity.x -= 2 * dot * normal.x;
        velocity.y -= 2 * dot * normal.y;
        normalizeVelocity();
    }

    void Ball::increaseSpeedFactor(float value) {
        speedFactor = std::min(speedFactor + value, Config::Physics::MAX_SPEED_FACTOR);
        normalizeVelocity();
    }

    void Ball::resetSpeedFactor() {
        speedFactor = 1.0f;
        normalizeVelocity();
    }

    float Ball::getSpeedFactor() const {
        return speedFactor;
    }

    float Ball::getBaseSpeed() const {
        return baseSpeed;
    }

    sf::Sprite& Ball::getSprite() {
        return sprite;
    }

    void Ball::savePreviousPosition() {
        previousPosition = sprite.getPosition();
    }

    void Ball::normalizeVelocity() {
        float currentSpeed = std::hypot(velocity.x, velocity.y);
        if (currentSpeed > 0.0f) {
            float targetSpeed = baseSpeed * speedFactor;
            velocity = (velocity / currentSpeed) * targetSpeed;
        }
    }
    
    bool Ball::getIsOnPaddle() const { return isOnPaddle; }
    void Ball::updateOnPaddle(float paddleCenterX, float paddleY) {
        if (isOnPaddle) {
            setPosition({ paddleCenterX , paddleY - getBounds().height });
        }
    }

    void Ball::launch() {
        if (isOnPaddle) {
            isOnPaddle = false;

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> angleDist(-45.0f, 45.0f);
            float angleRad = angleDist(gen) * Config::Physics::DEG_TO_RAD;

            float speed = baseSpeed * speedFactor;
            velocity = {
                speed * std::sin(angleRad),
                -speed * std::cos(angleRad)
            };
        }
    }

} // namespace Arkanoid
