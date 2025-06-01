#pragma once
#include "PowerUpEffect.h"

namespace Arkanoid {

    class ExtraLifeEffect : public PowerUpEffect {
    public:
        ExtraLifeEffect();
        ~ExtraLifeEffect() = default;

        void apply(Ball& ball, Paddle& paddle, std::function<void()> gameCallback) override;
        void update(float deltaTime) override;
        void remove() override;
        bool isTemporary() const override;
        float getDuration() const override;
        bool isFinished() const override;
        std::string getDescription() const override;
        PowerUpType getType() const override;
        float getRemainingTime() const override;
    private:
        bool finished;
    };

} // namespace Arkanoid
