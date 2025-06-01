#include "ExpandPaddleEffect.h"

namespace Arkanoid {

    ExpandPaddleEffect::ExpandPaddleEffect(float duration, float expansionFactor)
        : duration(duration),
        timeRemaining(duration),
        expansionFactor(expansionFactor),
        applied(false),
        finished(false),
        affectedPaddle(nullptr) {
    }
    
    void ExpandPaddleEffect::apply(Ball& ball, Paddle& paddle, std::function<void()> gameCallback) {
        if (applied) return;

        affectedPaddle = &paddle;

        if (applied) return;
        affectedPaddle = &paddle;

        // Всегда используем базовый размер
        sf::Vector2f newScale = paddle.getOriginalScale();
        newScale.x *= expansionFactor;
        paddle.getSprite().setScale(newScale);

        applied = true;
        timeRemaining = duration;
    }

    void ExpandPaddleEffect::update(float deltaTime) {
        if (!applied || finished) return;

        timeRemaining -= deltaTime;

        if (timeRemaining <= 0.0f) {
            remove();
        }
    }

    void ExpandPaddleEffect::remove() {
        if (!applied || !affectedPaddle || finished) return;

        // Возвращаем оригинальный размер
        affectedPaddle->resetToBaseScale();
        finished = true;
        affectedPaddle = nullptr;
    }

    bool ExpandPaddleEffect::isTemporary() const {
        return true;
    }

    float ExpandPaddleEffect::getDuration() const {
        return duration;
    }

    bool ExpandPaddleEffect::isFinished() const {
        return finished;
    }

    std::string ExpandPaddleEffect::getDescription() const {
        return "Expanded Paddle";
    }

    PowerUpType ExpandPaddleEffect::getType() const {
        return PowerUpType::ExpandPaddle;
    }

    void ExpandPaddleEffect::reset() {
        timeRemaining = duration;
        applied = false;
        finished = false;
        affectedPaddle = nullptr;
    }

    float ExpandPaddleEffect::getRemainingTime() const {
        return timeRemaining;
    }

} // namespace Arkanoid
