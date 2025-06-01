#pragma once
#include "BasePowerUp.h"

namespace Arkanoid {

    class ExtraLifePowerUp : public BasePowerUp {
    public:
        ExtraLifePowerUp(float x, float y);
        ~ExtraLifePowerUp() = default;

        std::unique_ptr<PowerUpEffect> createEffect() override;

    protected:
        void setupAppearance() override;
    };

} // namespace Arkanoid
