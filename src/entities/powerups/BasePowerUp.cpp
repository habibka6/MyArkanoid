#include "BasePowerUp.h"

namespace Arkanoid {

    void BasePowerUp::setupAppearance() {
        // Базовая настройка внешнего вида
        shape.setSize(sf::Vector2f(30, 30));
        shape.setOutlineThickness(1);
        shape.setOutlineColor(sf::Color::White);
        // Устанавливаем цвет в зависимости от типа
        switch (powerUpType) {
      
        case PowerUpType::ExpandPaddle:
            shape.setFillColor(sf::Color::Green);
            break;
        case PowerUpType::ExtraLife:
            shape.setFillColor(sf::Color::Red);
            break;
        case PowerUpType::ShrinkPaddle:
            shape.setFillColor(sf::Color::Yellow);
            break;
        case PowerUpType::SlowBall:
            shape.setFillColor(sf::Color::Blue);
            break;
        default:
            shape.setFillColor(sf::Color::Cyan);
            break;
        }

        // Центрируем origin
        shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);
    }

    BasePowerUp::BasePowerUp(float x, float y, PowerUpType type)
        : powerUpType(type),
        active(true),
        fallSpeed(Config::PowerUp::FALL_SPEED) {
        setupAppearance();
        setPosition({ x, y });
        setVelocity({ 0, fallSpeed });
    }

    void BasePowerUp::update(float deltaTime) {
        if (!active) return;

        move(velocity * deltaTime);
    }

    void BasePowerUp::draw(sf::RenderWindow& window) const {
        if (active) {
            window.draw(shape);
        }
    }

    sf::Vector2f BasePowerUp::getPosition() const {
        return shape.getPosition();
    }

    sf::FloatRect BasePowerUp::getBounds() const {
        return shape.getGlobalBounds();
    }

    void BasePowerUp::setPosition(const sf::Vector2f& position) {
        shape.setPosition(position);
    }

    EntityType BasePowerUp::getType() const {
        return EntityType::PowerUp;
    }

    bool BasePowerUp::isActive() const {
        return active;
    }

    void BasePowerUp::setActive(bool active) {
        this->active = active;
    }

    sf::Vector2f BasePowerUp::getVelocity() const {
        return velocity;
    }

    void BasePowerUp::setVelocity(const sf::Vector2f& velocity) {
        this->velocity = velocity;
    }

    void BasePowerUp::move(const sf::Vector2f& offset) {
        shape.move(offset);
    }

    PowerUpType BasePowerUp::getPowerUpType() const {
        return powerUpType;
    }

    bool BasePowerUp::isOutOfBounds(float windowHeight) const {
        return getPosition().y > windowHeight;
    }



} // namespace Arkanoid
