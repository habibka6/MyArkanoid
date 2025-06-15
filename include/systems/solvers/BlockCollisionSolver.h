#pragma once
#include "Config.h"
#include "BaseBlock.h"
#include "Ball.h"
#include <SFML/Graphics.hpp>
#include <utility>

namespace Arkanoid {

    class BlockCollisionSolver {
    public:
        bool checkBlockCollision(Ball& ball, BaseBlock& block) const;
        void resolveBlockCollision(Ball& ball, BaseBlock& block, float deltaTime);

        enum class CollisionSide { Top, Bottom, Left, Right };
        static void ensureMinimumVerticalVelocity(Ball& ball, float minRatio = Config::Physics::MIN_DIRECTSPEED_RATIO);
        static void correctPosition(Ball& ball, const sf::FloatRect& blockBounds, const sf::Vector2f& normal, float separation = 0.1f);
        static std::pair<sf::Vector2f, float> sweepCollision(
            const sf::Vector2f& startPos, const sf::Vector2f& velocity, float radius,
            const sf::FloatRect& blockBounds, float deltaTime);
    private: 

        static CollisionSide determineCollisionSide(const sf::Vector2f& ballPos, const sf::FloatRect& blockBounds);
        static sf::Vector2f calculateNormal(const sf::Vector2f& ballPos, const sf::FloatRect& blockBounds);
        
        
    };

}
