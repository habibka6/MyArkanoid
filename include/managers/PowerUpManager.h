#pragma once
#include "BasePowerUp.h"
#include "PowerUpEffect.h"
#include "Paddle.h"
#include "Ball.h"
#include <vector>
#include <memory>
#include <functional>
#include <random>

namespace Arkanoid
{
    class PowerUpManager
    {
    public:
        PowerUpManager();
        ~PowerUpManager() = default;

        void spawnPowerUp(const sf::Vector2f& position);
        void update(float deltaTime, Paddle& paddle, Ball& ball);
        void render(sf::RenderWindow& window);
        void reset();
        const std::vector<std::unique_ptr<PowerUpEffect>>& getActiveEffects() const;

        void setExtraLifeCallback(std::function<void()> callback);

        bool isTemporaryEffectActive() const;

    private:
        std::vector<std::unique_ptr<BasePowerUp>> powerups;
        std::vector<std::unique_ptr<PowerUpEffect>> activeEffects;
        std::function<void()> extraLifeCallback;

        void applyPowerUpEffect(BasePowerUp& powerup, Paddle& paddle, Ball& ball);
        void cancelEffect(PowerUpType type);

        


    };
}