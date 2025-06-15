#include "ExpandPaddlePowerUp.h"

namespace Arkanoid {

    ExpandPaddlePowerUp::ExpandPaddlePowerUp(float x, float y)
        : PowerUp(x, y)
    {
        setupAppearance();
    }

    void ExpandPaddlePowerUp::setupAppearance() {
        shape.setFillColor(sf::Color::Green);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::White);
    }

    PowerUpType ExpandPaddlePowerUp::getPowerUpType() const {
        return PowerUpType::ExpandPaddle;
    }

    std::unique_ptr<PowerUpEffect> ExpandPaddlePowerUp::createEffect() {
        return std::make_unique<ScalePaddleEffect>(10.0f, Config::Paddle::EXPAND_FACTOR);
    }

} // namespace Arkanoid
