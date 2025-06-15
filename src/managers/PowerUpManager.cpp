#include "PowerUpManager.h"
#include "Config.h"
#include "SoundManager.h"
#include <algorithm>
#include <random>

namespace Arkanoid
{
    PowerUpManager::PowerUpManager(std::vector<std::unique_ptr<PowerUp>>& powerups)
        : powerups(&powerups) {
    }

    void PowerUpManager::spawnPowerUp(const sf::Vector2f& position)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dis(0.0f, 1.0f);

        if (!isTemporaryEffectActive() && dis(gen) < Config::PowerUp::SPAWN_CHANCE)
        {
            static std::uniform_int_distribution<int> dist(0, 3);
            PowerUpType type = static_cast<PowerUpType>(dist(gen));
            auto powerup = factory.createPowerUp(type, position.x, position.y);
            if (powerup)
                powerups->push_back(std::move(powerup));
        }
    }

    void PowerUpManager::update(float deltaTime, Paddle& paddle, Ball& ball) {
        for (auto it = powerups->begin(); it != powerups->end(); ) {
            auto& powerup = *it;

            if (powerup->isActive()) {
                powerup->update(deltaTime);  
            }

            if (!powerup->isActive() || powerup->getPosition().y > Config::Window::HEIGHT) {
                it = powerups->erase(it);
            }
            else {
                ++it;
            }
        }

        for (auto it = activeEffects.begin(); it != activeEffects.end(); ) {
            auto& effect = *it;
            effect->update(deltaTime);
            if (effect->isFinished()) {
                it = activeEffects.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    void PowerUpManager::render(sf::RenderWindow& window)
    {
        for (auto& powerup : *powerups)
        {
            if (powerup->isActive())
            {
                powerup->draw(window);
            }
        }
    }

    void PowerUpManager::applyPowerUpEffect(PowerUp& powerup, Paddle& paddle, Ball& ball)
    {
        switch (powerup.getPowerUpType())
        {
        case PowerUpType::ExpandPaddle:
            cancelEffect(PowerUpType::ShrinkPaddle);
            cancelEffect(PowerUpType::ExpandPaddle);
            break;
        case PowerUpType::ShrinkPaddle:
            cancelEffect(PowerUpType::ExpandPaddle);
            cancelEffect(PowerUpType::ShrinkPaddle);
            break;
        case PowerUpType::SlowBall:
            cancelEffect(PowerUpType::SlowBall);
            break;
        default:
            break;
        }

        auto effect = powerup.createEffect();

        if (powerup.getPowerUpType() == PowerUpType::ExtraLife && extraLifeCallback)
        {
            effect->apply(ball, paddle, extraLifeCallback);
        }
        else
        {
            effect->apply(ball, paddle, nullptr);
        }

        activeEffects.push_back(std::move(effect));
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
        powerups->clear();

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
