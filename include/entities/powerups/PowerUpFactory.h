#pragma once
#include <functional>
#include <unordered_map>
#include <memory>
#include "PowerUp.h"


namespace Arkanoid {

    class PowerUpFactory {
    public:
        using Creator = std::function<std::unique_ptr<PowerUp>(float, float)>;

        PowerUpFactory();

        std::unique_ptr<PowerUp> createPowerUp(PowerUpType type, float x, float y) const;

    private:
        std::unordered_map<PowerUpType, Creator> creators;
    };

} // namespace Arkanoid
