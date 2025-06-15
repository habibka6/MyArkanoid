#include "PowerUpFactory.h"
#include "ShrinkPaddlePowerUp.h"
#include "ExpandPaddlePowerUp.h"
#include "SlowBallPowerUp.h"
#include "ExtraLifePowerUp.h"

namespace Arkanoid {

    PowerUpFactory::PowerUpFactory() {
        creators[PowerUpType::ShrinkPaddle] = [](float x, float y) {
            return std::make_unique<ShrinkPaddlePowerUp>(x, y);
            };

        creators[PowerUpType::ExpandPaddle] = [](float x, float y) {
            return std::make_unique<ExpandPaddlePowerUp>(x, y);
            };

        creators[PowerUpType::SlowBall] = [](float x, float y) {
            return std::make_unique<SlowBallPowerUp>(x, y);
            };

        creators[PowerUpType::ExtraLife] = [](float x, float y) {
            return std::make_unique<ExtraLifePowerUp>(x, y);
            };
    }

    std::unique_ptr<PowerUp> PowerUpFactory::createPowerUp(
        PowerUpType type,
        float x,
        float y
    ) const {
        auto it = creators.find(type);
        if (it != creators.end()) {
            return it->second(x, y);
        }
        return nullptr; // Или бросить исключение
    }

} // namespace Arkanoid