#include "ExtraLifePowerUp.h"
#include "ExtraLifeEffect.h"

namespace Arkanoid {

    ExtraLifePowerUp::ExtraLifePowerUp(float x, float y)
        : BasePowerUp(x, y, PowerUpType::ExtraLife) {
    }

    std::unique_ptr<PowerUpEffect> ExtraLifePowerUp::createEffect() {
        return std::make_unique<ExtraLifeEffect>();
    }

    void ExtraLifePowerUp::setupAppearance() {
        shape.setSize(sf::Vector2f(30, 30));
        shape.setFillColor(sf::Color::Red);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::White);
        shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);
    }

} // namespace Arkanoid