#include "ExpandPaddlePowerUp.h"
#include "ExpandPaddleEffect.h"

namespace Arkanoid {

    ExpandPaddlePowerUp::ExpandPaddlePowerUp(float x, float y)
        : BasePowerUp(x, y, PowerUpType::ExpandPaddle) {
    }

    std::unique_ptr<PowerUpEffect> ExpandPaddlePowerUp::createEffect() {
        return std::make_unique<ExpandPaddleEffect>();
    }

    void ExpandPaddlePowerUp::setupAppearance() {
        shape.setSize(sf::Vector2f(30, 30));
        shape.setFillColor(sf::Color::Green);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::White);
        shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);
    }

} // namespace Arkanoi