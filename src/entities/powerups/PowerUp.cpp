#include "PowerUp.h"

namespace Arkanoid {

    PowerUp::PowerUp(float x, float y) : active(true), fallSpeed(Config::PowerUp::FALL_SPEED)
    {
        shape.setSize(sf::Vector2f(30, 30));
        shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);
        setPosition({ x, y });
    }

    void PowerUp::update(float deltaTime) {
        if (!active) return;
        move({ 0, fallSpeed * deltaTime });
        if (getPosition().y > Config::Window::HEIGHT)
            active = false;
    }

    void PowerUp::draw(sf::RenderWindow& window) const {
        if (active)
            window.draw(shape);
    }

    sf::Vector2f PowerUp::getPosition() const { return shape.getPosition(); }
    void PowerUp::setPosition(const sf::Vector2f& pos) { shape.setPosition(pos); }
    sf::FloatRect PowerUp::getBounds() const { return shape.getGlobalBounds(); }
    EntityType PowerUp::getType() const { return EntityType::PowerUp; }
    bool PowerUp::isActive() const { return active; }
    void PowerUp::setActive(bool a) { active = a; }
    sf::Vector2f PowerUp::getVelocity() const { return { 0, fallSpeed }; }
    void PowerUp::setVelocity(const sf::Vector2f& v) { fallSpeed = v.y; }
    void PowerUp::move(const sf::Vector2f& offset) { shape.move(offset); }

} // namespace Arkanoid
