#include "ShrinkPaddleEffect.h"

namespace Arkanoid {

    ShrinkPaddleEffect::ShrinkPaddleEffect(float duration, float shrinkFactor)
        : duration(duration),
        timeRemaining(duration),
        shrinkFactor(shrinkFactor),
        applied(false),
        finished(false),
        affectedPaddle(nullptr) {
    }

    void ShrinkPaddleEffect::apply(Ball& ball, Paddle& paddle, std::function<void()> gameCallback) {
        if (applied) return;

        affectedPaddle = &paddle;

        sf::Vector2f newScale = paddle.getOriginalScale();
        newScale.x *= shrinkFactor;
        paddle.getSprite().setScale(newScale);
        applied = true;
        timeRemaining = duration;
    }

    void ShrinkPaddleEffect::update(float deltaTime) {
        if (!applied || finished) return;

        timeRemaining -= deltaTime;

        if (timeRemaining <= 0.0f) {
            remove();
        }
    }

    void ShrinkPaddleEffect::remove() {
        if (!applied || !affectedPaddle || finished) return;

        affectedPaddle->resetToBaseScale(); // Возвращаем к базовому
        finished = true;
    }

    bool ShrinkPaddleEffect::isTemporary() const {
        return true;
    }

    float ShrinkPaddleEffect::getDuration() const {
        return duration;
    }

    bool ShrinkPaddleEffect::isFinished() const {
        return finished;
    }

    std::string ShrinkPaddleEffect::getDescription() const {
        return "Shrinked Paddle";
    }

    PowerUpType ShrinkPaddleEffect::getType() const {
        return PowerUpType::ShrinkPaddle;
    }

    void ShrinkPaddleEffect::reset() {
        timeRemaining = duration;
        applied = false;
        finished = false;
        affectedPaddle = nullptr;
    }

    float ShrinkPaddleEffect::getRemainingTime() const {
        return timeRemaining;
    }

} // namespace Arkanoid
