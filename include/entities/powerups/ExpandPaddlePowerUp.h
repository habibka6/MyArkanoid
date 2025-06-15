#pragma once
#include "PowerUp.h"
#include "ScalePaddleEffect.h"

namespace Arkanoid {

    class ExpandPaddlePowerUp : public PowerUp {
    public:
        ExpandPaddlePowerUp(float x, float y);

        void setupAppearance() override;
        PowerUpType getPowerUpType() const override;
        std::unique_ptr<PowerUpEffect> createEffect() override;
    };

} // namespace Arkanoid
