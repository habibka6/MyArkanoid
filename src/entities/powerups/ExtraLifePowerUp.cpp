#include "ExtraLifePowerUp.h"

namespace Arkanoid {

    ExtraLifePowerUp::ExtraLifePowerUp(float x, float y)
        : PowerUp(x, y)
    {
        setupAppearance();
    }

    void ExtraLifePowerUp::setupAppearance() {
        shape.setFillColor(sf::Color::Red);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::White);
    }

    PowerUpType ExtraLifePowerUp::getPowerUpType() const {
        return PowerUpType::ExtraLife;
    }

    std::unique_ptr<PowerUpEffect> ExtraLifePowerUp::createEffect() {
        return std::make_unique<ExtraLifeEffect>();
    }

} // namespace Arkanoid
