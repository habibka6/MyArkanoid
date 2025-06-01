#pragma once
#include "PowerUpEffect.h"
#include <SFML/Graphics.hpp>

namespace Arkanoid {

    class ExpandPaddleEffect : public PowerUpEffect {
    public:
        ExpandPaddleEffect(float duration = 10.0f, float expansionFactor = 1.5f);
        ~ExpandPaddleEffect() = default;

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
        float getRemainingTime() const;

    private:
        float duration;
        float timeRemaining;
        float expansionFactor;
        bool applied;
        bool finished;

        Paddle* affectedPaddle;
    };

} // namespace Arkanoid
