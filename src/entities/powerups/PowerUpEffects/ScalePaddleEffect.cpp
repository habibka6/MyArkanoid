#include "ScalePaddleEffect.h"

namespace Arkanoid {

    ScalePaddleEffect::ScalePaddleEffect(float duration, float scaleFactor)
        : duration(duration),
        timeRemaining(duration),
        scaleFactor(scaleFactor),
        applied(false),
        finished(false),
        affectedPaddle(nullptr) {
    }

    void ScalePaddleEffect::apply(Ball& ball, Paddle& paddle, std::function<void()> gameCallback) {
        if (applied) return;

        affectedPaddle = &paddle;

        sf::Vector2f newScale = paddle.getOriginalScale();
        newScale.x *= scaleFactor;
        paddle.getSprite().setScale(newScale);
        applied = true;
        timeRemaining = duration;
    }

    void ScalePaddleEffect::update(float deltaTime) {
        if (!applied || finished) return;

        timeRemaining -= deltaTime;

        if (timeRemaining <= 0.0f) {
            remove();
        }
    }

    void ScalePaddleEffect::remove() {
        if (!applied || !affectedPaddle || finished) return;

        affectedPaddle->resetToBaseScale();
        finished = true;
    }

    bool ScalePaddleEffect::isTemporary() const {
        return true;
    }

    float ScalePaddleEffect::getDuration() const {
        return duration;
    }

    bool ScalePaddleEffect::isFinished() const {
        return finished;
    }

    std::string ScalePaddleEffect::getDescription() const {
        return "Scaled Paddle";
    }

    PowerUpType ScalePaddleEffect::getType() const {
        return PowerUpType::ShrinkPaddle;
    }

    void ScalePaddleEffect::reset() {
        timeRemaining = duration;
        applied = false;
        finished = false;
        affectedPaddle = nullptr;
    }

    float ScalePaddleEffect::getRemainingTime() const {
        return timeRemaining;
    }

} // namespace Arkanoid
