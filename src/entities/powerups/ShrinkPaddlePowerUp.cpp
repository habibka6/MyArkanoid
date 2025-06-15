// ShrinkPaddlePowerUp.cpp
#include "ShrinkPaddlePowerUp.h"

namespace Arkanoid {

    ShrinkPaddlePowerUp::ShrinkPaddlePowerUp(float x, float y)
        : PowerUp(x, y)
    {
        setupAppearance();
    }

    void ShrinkPaddlePowerUp::setupAppearance() {
        shape.setFillColor(sf::Color::Yellow);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::Black);
    }

    PowerUpType ShrinkPaddlePowerUp::getPowerUpType() const {
        return PowerUpType::ShrinkPaddle;
    }

    std::unique_ptr<PowerUpEffect> ShrinkPaddlePowerUp::createEffect() {
        return std::make_unique<ScalePaddleEffect>(10.0f, Config::Paddle::SHRINK_FACTOR);
    }

} // namespace Arkanoid
