#pragma once
#include "Ball.h"
#include "Paddle.h"
#include <functional>
#include <string>

namespace Arkanoid {
    enum class PowerUpType {
        ExtraLife,
        ExpandPaddle,
        SlowBall,
        ShrinkPaddle
    };
    class PowerUpEffect {
    public:
        virtual ~PowerUpEffect() = default;

        virtual void apply(Ball& ball, Paddle& paddle, std::function<void()> gameCallback) = 0;
        virtual void update(float deltaTime) = 0;
        virtual void remove() = 0;
        virtual bool isTemporary() const = 0;
        virtual float getDuration() const = 0;
        virtual bool isFinished() const = 0;
        virtual std::string getDescription() const = 0;
        virtual PowerUpType getType() const = 0;
        virtual  float getRemainingTime() const = 0;

    protected:
        PowerUpEffect() = default;
    };
    ;
} // namespace Arkanoid
