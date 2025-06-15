#include "Rock.h"
#include "Config.h"
namespace Arkanoid {

    Rock::Rock(float x, float y) : active(true) {
        setupRock();
        setPosition({ x, y });
    }

    void Rock::setupRock() {
        shape.setSize(sf::Vector2f(Config::Block::WIDTH, Config::Block::HEIGHT));
        shape.setFillColor(sf::Color(128, 128, 128)); 
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::Black);
    }

    void Rock::update(float deltaTime) { }

    void Rock::draw(sf::RenderWindow& window) const {
        if (active) {
            window.draw(shape);
        }
    }

    sf::Vector2f Rock::getPosition() const {
        return shape.getPosition();
    }

    sf::FloatRect Rock::getBounds() const {
        return shape.getGlobalBounds();
    }

    void Rock::setPosition(const sf::Vector2f& position) {
        shape.setPosition(position);
    }

    bool Rock::isActive() const {
        return active;
    }

    void Rock::setActive(bool active) {
        this->active = active;
    }

    void Rock::hit() { }

    bool Rock::isDestroyed() const {
        return false;
    }

    int Rock::getPoints() const {
        return 0;
    }

    BlockType Rock::getBlockType() const {
        return BlockType::Rock;
    }

    sf::Color Rock::getColor() const {
        return shape.getFillColor();
    }

} // namespace Arkanoid
