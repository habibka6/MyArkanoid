#pragma once
#include "BasePowerUp.h"

namespace Arkanoid {

    class ExpandPaddlePowerUp : public BasePowerUp {
    public:
        ExpandPaddlePowerUp(float x, float y);
        ~ExpandPaddlePowerUp() = default;

        std::unique_ptr<PowerUpEffect> createEffect() override;

    protected:
        void setupAppearance() override;
    };

} // namespace Arkanoid
