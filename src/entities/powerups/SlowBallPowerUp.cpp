
#include "SlowBallPowerUp.h"

namespace Arkanoid {

    SlowBallPowerUp::SlowBallPowerUp(float x, float y)
        : PowerUp(x, y)
    {
        setupAppearance();
    }

    void SlowBallPowerUp::setupAppearance() {
        shape.setFillColor(sf::Color::Blue);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::White);
    }

    PowerUpType SlowBallPowerUp::getPowerUpType() const {
        return PowerUpType::SlowBall;
    }

    std::unique_ptr<PowerUpEffect> SlowBallPowerUp::createEffect() {
        return std::make_unique<SlowBallEffect>();
    }

} // namespace Arkanoid
