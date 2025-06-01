#pragma once
#include "PowerUpEffect.h"
#include <SFML/Graphics.hpp>

namespace Arkanoid {

    class ShrinkPaddleEffect : public PowerUpEffect {
    public:
        ShrinkPaddleEffect(float duration = 8.0f, float shrinkFactor = 0.7f);
        ~ShrinkPaddleEffect() = default;

        void apply(Ball& ball, Paddle& paddle, std::function<void()> gameCallback) override;
        void update(float deltaTime) override;
        void remove() override;
        bool isTemporary() const override;
        float getDuration() const override;
        bool isFinished() const override;
        std::string getDescription() const override;
        PowerUpType getType() const override;

        // Дополнительные методы
        void reset();
        float getRemainingTime() const override;

    private:
        float duration;
        float timeRemaining;
        float shrinkFactor;
        bool applied;
        bool finished;

        Paddle* affectedPaddle;
    };

} // namespace Arkanoid
