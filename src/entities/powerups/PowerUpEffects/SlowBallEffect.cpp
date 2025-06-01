#include "SlowBallEffect.h"

namespace Arkanoid {

    SlowBallEffect::SlowBallEffect(float duration, float slowFactor)
        : duration(duration),
        timeRemaining(duration),
        slowFactor(slowFactor),
        originalSpeedFactor(1.0f),
        applied(false),
        finished(false),
        affectedBall(nullptr) {
    }

    void SlowBallEffect::apply(Ball& ball, Paddle& paddle, std::function<void()> gameCallback) {
        if (applied) return;

        affectedBall = &ball;

        // Сохраняем оригинальную скорость
        originalSpeedFactor = ball.getSpeedFactor();

        // Применяем замедление
        float newSpeedFactor = originalSpeedFactor * slowFactor;

        // Получаем текущую скорость и направление
        sf::Vector2f currentVelocity = ball.getVelocity();
        float currentSpeed = std::sqrt(currentVelocity.x * currentVelocity.x + currentVelocity.y * currentVelocity.y);

        if (currentSpeed > 0.0f) {
            // Нормализуем направление и применяем новую скорость
            sf::Vector2f direction = currentVelocity / currentSpeed;
            float newSpeed = ball.getBaseSpeed() * newSpeedFactor;
            ball.setVelocity(direction * newSpeed);
        }

        applied = true;
        timeRemaining = duration;
    }

    void SlowBallEffect::update(float deltaTime) {
        if (!applied || finished) return;

        timeRemaining -= deltaTime;

        if (timeRemaining <= 0.0f) {
            remove();
        }
    }

    void SlowBallEffect::remove() {
        if (!applied || !affectedBall || finished) return;

        // Возвращаем оригинальную скорость
        sf::Vector2f currentVelocity = affectedBall->getVelocity();
        float currentSpeed = std::sqrt(currentVelocity.x * currentVelocity.x + currentVelocity.y * currentVelocity.y);

        if (currentSpeed > 0.0f) {
            sf::Vector2f direction = currentVelocity / currentSpeed;
            float restoredSpeed = affectedBall->getBaseSpeed() * originalSpeedFactor;
            affectedBall->setVelocity(direction * restoredSpeed);
        }

        finished = true;
        affectedBall = nullptr;
    }

    bool SlowBallEffect::isTemporary() const {
        return true;
    }

    float SlowBallEffect::getDuration() const {
        return duration;
    }

    bool SlowBallEffect::isFinished() const {
        return finished;
    }

    std::string SlowBallEffect::getDescription() const {
        return "Slow Ball";
    }

    PowerUpType SlowBallEffect::getType() const {
        return PowerUpType::SlowBall;
    }

    void SlowBallEffect::reset() {
        timeRemaining = duration;
        applied = false;
        finished = false;
        affectedBall = nullptr;
    }

    float SlowBallEffect::getRemainingTime() const {
        return timeRemaining;
    }

} // namespace Arkanoid
