#pragma once
#include "BasePowerUp.h"

namespace Arkanoid {

    class SlowBallPowerUp : public BasePowerUp {
    public:
        SlowBallPowerUp(float x, float y);
        ~SlowBallPowerUp() = default;

        std::unique_ptr<PowerUpEffect> createEffect() override;

    protected:
        void setupAppearance() override;
    };

} // namespace Arkanoid
