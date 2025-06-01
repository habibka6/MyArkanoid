#pragma once
#include "BasePowerUp.h"

namespace Arkanoid {

    class ShrinkPaddlePowerUp : public BasePowerUp {
    public:
        ShrinkPaddlePowerUp(float x, float y);
        ~ShrinkPaddlePowerUp() = default;

        std::unique_ptr<PowerUpEffect> createEffect() override;

    protected:
        void setupAppearance() override;
    };

} // namespace Arkanoid
