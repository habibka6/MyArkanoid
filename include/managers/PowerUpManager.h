#pragma once
#include "PowerUp.h"
#include "PowerUpEffect.h"
#include "PowerUpFactory.h"
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
        explicit PowerUpManager(std::vector<std::unique_ptr<PowerUp>>& powerups);
        ~PowerUpManager() = default;

        void spawnPowerUp(const sf::Vector2f& position);
        void applyPowerUpEffect(PowerUp& powerup, Paddle& paddle, Ball& ball);
        void update(float deltaTime, Paddle& paddle, Ball& ball);
        void render(sf::RenderWindow& window);
        void reset();
        const std::vector<std::unique_ptr<PowerUpEffect>>& getActiveEffects() const;

        void setExtraLifeCallback(std::function<void()> callback);

        bool isTemporaryEffectActive() const;

    private:
        std::vector<std::unique_ptr<PowerUp>>* powerups;
        std::vector<std::unique_ptr<PowerUpEffect>> activeEffects;
        std::function<void()> extraLifeCallback;

        PowerUpFactory factory; 

        
        void cancelEffect(PowerUpType type);
    };
}
