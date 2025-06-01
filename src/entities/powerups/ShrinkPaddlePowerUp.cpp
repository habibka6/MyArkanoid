#include "ShrinkPaddlePowerUp.h"
#include "ShrinkPaddleEffect.h"

namespace Arkanoid {

    ShrinkPaddlePowerUp::ShrinkPaddlePowerUp(float x, float y)
        : BasePowerUp(x, y, PowerUpType::ShrinkPaddle) {
    }

    std::unique_ptr<PowerUpEffect> ShrinkPaddlePowerUp::createEffect() {
        return std::make_unique<ShrinkPaddleEffect>();
    }

    void ShrinkPaddlePowerUp::setupAppearance() {
        shape.setSize(sf::Vector2f(30, 30));
        shape.setFillColor(sf::Color::Yellow);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::Black);
        shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);
    }

} // namespace Arkanoid