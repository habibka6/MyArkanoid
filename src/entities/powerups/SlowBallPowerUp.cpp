#include "SlowBallPowerUp.h"
#include "SlowBallEffect.h"

namespace Arkanoid {

    SlowBallPowerUp::SlowBallPowerUp(float x, float y)
        : BasePowerUp(x, y, PowerUpType::SlowBall) {
    }

    std::unique_ptr<PowerUpEffect> SlowBallPowerUp::createEffect() {
        return std::make_unique<SlowBallEffect>();
    }

    void SlowBallPowerUp::setupAppearance() {
        shape.setSize(sf::Vector2f(30, 30));
        shape.setFillColor(sf::Color::Magenta);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::White);
        shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);
    }

} // namespace Arkanoid