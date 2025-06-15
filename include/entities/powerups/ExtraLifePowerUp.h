#pragma once
#include "PowerUp.h"
#include "ExtraLifeEffect.h"

namespace Arkanoid {

    class ExtraLifePowerUp : public PowerUp {
    public:
        ExtraLifePowerUp(float x, float y);

        void setupAppearance() override;
        PowerUpType getPowerUpType() const override;
        std::unique_ptr<PowerUpEffect> createEffect() override;
    };

} // namespace Arkanoid
