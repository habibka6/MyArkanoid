#pragma once
#include "PowerUp.h"
#include "SlowBallEffect.h"

namespace Arkanoid {

    class SlowBallPowerUp : public PowerUp {
    public:
        SlowBallPowerUp(float x, float y);

        void setupAppearance() override;
        PowerUpType getPowerUpType() const override;
        std::unique_ptr<PowerUpEffect> createEffect() override;
    };

} // namespace Arkanoid
