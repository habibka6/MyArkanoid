#include "ExtraLifeEffect.h"

namespace Arkanoid {

    ExtraLifeEffect::ExtraLifeEffect()
        : finished(false) {
    }

    void ExtraLifeEffect::apply(Ball& ball, Paddle& paddle, std::function<void()> gameCallback) {
        if (gameCallback) {
            gameCallback();
        }
        finished = true;
    }

    void ExtraLifeEffect::update(float deltaTime) {}

    void ExtraLifeEffect::remove() { finished = true; }

    bool ExtraLifeEffect::isTemporary() const {
        return false; 
    }

    float ExtraLifeEffect::getDuration() const {
        return 0.0f;
    }

    bool ExtraLifeEffect::isFinished() const {
        return finished;
    }

    std::string ExtraLifeEffect::getDescription() const {
        return "Extra Life";
    }

    PowerUpType ExtraLifeEffect::getType() const {
        return PowerUpType::ExtraLife;
    }
    float ExtraLifeEffect::getRemainingTime() const {
        return 0.0;
    }
} // namespace Arkanoid
