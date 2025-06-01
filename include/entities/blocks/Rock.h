#pragma once
#include "BaseBlock.h"

namespace Arkanoid {

    class Rock : public BaseBlock {
    public:
        Rock(float x, float y);
        ~Rock() = default;

        // Entity interface
        void update(float deltaTime) override;
        void draw(sf::RenderWindow& window) const override;
        sf::Vector2f getPosition() const override;
        sf::FloatRect getBounds() const override;
        void setPosition(const sf::Vector2f& position) override;
        bool isActive() const override;
        void setActive(bool active) override;

        // BaseBlock interface
        bool hit() override;
        bool isDestroyed() const override;
        int getPoints() const override;
        BlockType getBlockType() const override;
        sf::Color getColor() const override;

    private:
        sf::RectangleShape shape;
        bool active;

        void setupRock();
    };

} // namespace Arkanoid
