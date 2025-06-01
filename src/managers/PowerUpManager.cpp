#include "PowerUpManager.h"
#include "ExtraLifePowerUp.h"
#include "ExpandPaddlePowerUp.h"
#include "ShrinkPaddlePowerUp.h"
#include "SlowBallPowerUp.h"
#include "Config.h"
#include "SoundManager.h"
#include <algorithm>

namespace Arkanoid
{
    PowerUpManager::PowerUpManager() = default;

    void PowerUpManager::spawnPowerUp(const sf::Vector2f& position)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> dist(0, 3);

        switch (dist(gen))
        {
        case 0:
            powerups.push_back(std::make_unique<ExtraLifePowerUp>(position.x, position.y));
            break;
        case 1:
            powerups.push_back(std::make_unique<ExpandPaddlePowerUp>(position.x, position.y));
            break;
        case 2:
            powerups.push_back(std::make_unique<ShrinkPaddlePowerUp>(position.x, position.y));
            break;
        case 3:
            powerups.push_back(std::make_unique<SlowBallPowerUp>(position.x, position.y));
            break;
        }
    }

    void PowerUpManager::update(float deltaTime, Paddle& paddle, Ball& ball)
    {
        // Update active powerups
        for (auto it = powerups.begin(); it != powerups.end();)
        {
            auto& powerup = *it;

            if (powerup->isActive())
            {
                powerup->update(deltaTime);

                // Check collision with paddle
                if (paddle.getBounds().intersects(powerup->getBounds()))
                {
                    applyPowerUpEffect(*powerup, paddle, ball);
                    it = powerups.erase(it);
                    continue;
                }

                // Remove if out of screen
                if (powerup->isOutOfBounds(Config::Window::HEIGHT))
                {
                    it = powerups.erase(it);
                    continue;
                }
            }
            ++it;
        }

        // Update active effects
        for (auto it = activeEffects.begin(); it != activeEffects.end();)
        {
            auto& effect = *it;
            effect->update(deltaTime);

            if (effect->isFinished())
            {
                it = activeEffects.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void PowerUpManager::render(sf::RenderWindow& window)
    {
        for (auto& powerup : powerups)
        {
            if (powerup->isActive())
            {
                powerup->draw(window);
            }
        }
    }

    void PowerUpManager::applyPowerUpEffect(BasePowerUp& powerup, Paddle& paddle, Ball& ball)
    {
        // Handle conflicting effects
        switch (powerup.getPowerUpType())
        {
        case PowerUpType::ExpandPaddle:
            cancelEffect(PowerUpType::ShrinkPaddle);
            break;
        case PowerUpType::ShrinkPaddle:
            cancelEffect(PowerUpType::ExpandPaddle);
            break;
        }

        // Create and apply effect
        auto effect = powerup.createEffect();

        // Special handling for ExtraLife
        if (powerup.getPowerUpType() == PowerUpType::ExtraLife && extraLifeCallback)
        {
            effect->apply(ball, paddle, extraLifeCallback);
        }
        else
        {
            effect->apply(ball, paddle, nullptr);
        }

        activeEffects.push_back(std::move(effect));
        SoundManager::getInstance().playSound(SoundType::PowerUpCollect);
    }

    void PowerUpManager::cancelEffect(PowerUpType type)
    {
        for (auto it = activeEffects.begin(); it != activeEffects.end();)
        {
            if ((*it)->getType() == type)
            {
                (*it)->remove();
                it = activeEffects.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void PowerUpManager::reset()
    {
        powerups.clear();

        for (auto& effect : activeEffects)
        {
            effect->remove();
        }
        activeEffects.clear();
    }

    const std::vector<std::unique_ptr<PowerUpEffect>>& PowerUpManager::getActiveEffects() const
    {
        return activeEffects;
    }

    void PowerUpManager::setExtraLifeCallback(std::function<void()> callback)
    {
        extraLifeCallback = callback;
    }

    bool PowerUpManager::isTemporaryEffectActive() const {
        for (const auto& effect : activeEffects) {
            if (effect->isTemporary() && !effect->isFinished()) {
                return true;
            }
        }
        return false;
    }
}