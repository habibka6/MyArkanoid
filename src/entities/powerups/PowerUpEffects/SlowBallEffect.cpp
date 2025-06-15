#include "SlowBallEffect.h"

namespace Arkanoid {

    SlowBallEffect::SlowBallEffect(float duration, float slowFactor)
        : duration(duration),
        timeRemaining(duration),
        slowFactor(slowFactor),
        savedSpeedFactor(1.0f),
        finished(false),
        affectedBall(nullptr) {
    }

    void SlowBallEffect::apply(Ball& ball, Paddle&, std::function<void()>) {
        affectedBall = &ball;
        savedSpeedFactor = ball.getSpeedFactor(); 
        ball.setSpeedFactor(slowFactor);          
        timeRemaining = duration;
        finished = false;
    }

    void SlowBallEffect::update(float deltaTime) {
        if (finished || !affectedBall) return;
        timeRemaining -= deltaTime;
        if (timeRemaining <= 0.0f) {
            remove();
        }
    }

    void SlowBallEffect::remove() {
        if (finished || !affectedBall) return;
        affectedBall->setSpeedFactor(savedSpeedFactor); 
        finished = true;
        affectedBall = nullptr;
    }

    bool SlowBallEffect::isTemporary() const { return true; }
    float SlowBallEffect::getDuration() const { return duration; }
    bool SlowBallEffect::isFinished() const { return finished; }
    std::string SlowBallEffect::getDescription() const { return "Slow Ball"; }
    PowerUpType SlowBallEffect::getType() const { return PowerUpType::SlowBall; }
    void SlowBallEffect::reset() {
        timeRemaining = duration;
        finished = false;
        affectedBall = nullptr;
    }
    float SlowBallEffect::getRemainingTime() const { return timeRemaining; }

} // namespace Arkanoid
