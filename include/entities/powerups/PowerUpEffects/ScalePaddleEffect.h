#pragma once
#include "PowerUpEffect.h"
#include <SFML/Graphics.hpp>

namespace Arkanoid {

    class ScalePaddleEffect : public PowerUpEffect {
    public:
        ScalePaddleEffect(float duration = 8.0f, float scaleFactor= 1.0f);
        ~ScalePaddleEffect() = default;

        void apply(Ball& ball, Paddle& paddle, std::function<void()> gameCallback) override;
        void update(float deltaTime) override;
        void remove() override;
        bool isTemporary() const override;
        float getDuration() const override;
        bool isFinished() const override;
        std::string getDescription() const override;
        PowerUpType getType() const override;
        float getRemainingTime() const override;

        void reset();

    private:
        float duration;
        float timeRemaining;
        float scaleFactor;
        bool applied;
        bool finished;

        Paddle* affectedPaddle;
    };

} // namespace Arkanoid
