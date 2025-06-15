#include "Block.h"

namespace Arkanoid {

    Block::Block(float x, float y, Type type)
        : blockType(type),
        destroyed(false),
        active(true) {
        setupBlock();
        setPosition({ x, y });
    }

    void Block::setupBlock() {
        shape.setSize(sf::Vector2f(Config::Block::WIDTH, Config::Block::HEIGHT));
        shape.setOutlineThickness(3);
        shape.setOutlineColor(sf::Color::Black);

        switch (blockType) {
        case Type::Green:
            maxHealth = currentHealth = 1;
            pointValue = 10;
            break;
        case Type::Yellow:
            maxHealth = currentHealth = 2;
            pointValue = 20;
            break;
        case Type::Red:
            maxHealth = currentHealth = 3;
            pointValue = 30;
            break;
        }

        updateAppearance();
    }

    void Block::update(float deltaTime) {}

    void Block::draw(sf::RenderWindow& window) const {
        if (active && !destroyed) {
            window.draw(shape);
        }
    }

    sf::Vector2f Block::getPosition() const {
        return shape.getPosition();
    }

    sf::FloatRect Block::getBounds() const {
        return shape.getGlobalBounds();
    }

    void Block::setPosition(const sf::Vector2f& position) {
        shape.setPosition(position);
    }

    bool Block::isActive() const {
        return active && !destroyed;
    }

    void Block::setActive(bool active) {
        this->active = active;
    }

    void Block::hit() {
        currentHealth--;
        updateAppearance();

        if (currentHealth <= 0) {
            destroyed = true;
        }
        
    }

    bool Block::isDestroyed() const {
        return destroyed;
    }

    int Block::getPoints() const {
        return pointValue;
    }

    BlockType Block::getBlockType() const {
        return BlockType::Standard;
    }

    sf::Color Block::getColor() const {
        return shape.getFillColor();
    }

    void Block::updateAppearance() {
        shape.setFillColor(getColorForType(blockType, currentHealth, maxHealth));
    }

    sf::Color Block::getColorForType(Type type, int health, int maxHealth) {
        float ratio = static_cast<float>(health) / maxHealth;

        switch (type) {
        case Type::Green:
            return sf::Color::Green;
        case Type::Yellow:
            if (health == maxHealth) {
                return sf::Color::Yellow;
            }
            else {
                return sf::Color(205, 160, 50); 
            }
        case Type::Red:
            return sf::Color(255 * ratio, 0, 0);
        }
        return sf::Color::White;
    }

} // namespace Arkanoid
