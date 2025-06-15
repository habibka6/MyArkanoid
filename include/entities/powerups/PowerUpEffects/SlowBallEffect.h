#pragma once
#include "PowerUpEffect.h"
#include "Ball.h"
#include "Paddle.h"
#include <functional>

namespace Arkanoid {

    class SlowBallEffect : public PowerUpEffect {
    public:
        SlowBallEffect(float duration = 8.0f, float slowFactor = 0.8f);
        ~SlowBallEffect() = default;

        void apply(Ball& ball, Paddle& paddle, std::function<void()> gameCallback) override;
        void update(float deltaTime) override;
        void remove() override;
        bool isTemporary() const override;
        float getDuration() const override;
        bool isFinished() const override;
        std::string getDescription() const override;
        PowerUpType getType() const override;
        void reset();
        float getRemainingTime() const override;

    private:
        float duration;
        float timeRemaining;
        float slowFactor;
        float savedSpeedFactor;
        bool finished;
        Ball* affectedBall;
    };

} // namespace Arkanoid
