#pragma once
#include "Config.h"
#include "BaseBlock.h"
#include "Ball.h"
#include <SFML/Graphics.hpp>
#include <utility>

namespace Arkanoid {

    class BlockCollisionSolver {
    public:
        // Проверяет столкновение мяча с блоком
        bool checkBlockCollision(Ball& ball, BaseBlock& block) const;

        // Разрешает столкновение (изменяет скорость/позицию мяча)
        void resolveBlockCollision(Ball& ball, BaseBlock& block, float deltaTime);

        // Сторона столкновения (используется для определения отскока)
        enum class CollisionSide { Top, Bottom, Left, Right };

        static void ensureMinimumDirectionVelocity(Ball& ball, float minRatio = Config::Physics::MIN_DIRECTSPEED_RATIO);

        // Корректирует позицию мяча после столкновения
        static void correctPosition(Ball& ball, const sf::FloatRect& blockBounds, const sf::Vector2f& normal, float separation = 0.2f);

        // Детектит первое касание мяча с блоком на его траектории, выдавая сторону удара и точный момент коллизии
        static std::pair<sf::Vector2f, float> sweepCollision(const sf::Vector2f& startPos, 
            const sf::Vector2f& velocity, float radius, const sf::FloatRect& blockBounds, float deltaTime);
    private: 
        // Определяет сторону столкновения мяча с блоком
        static CollisionSide determineCollisionSide(const sf::Vector2f& ballPos, const sf::FloatRect& blockBounds);

        // Вычисляет нормаль отскока
        static sf::Vector2f calculateNormal(const sf::Vector2f& ballPos, const sf::FloatRect& blockBounds);
        
        
    };

}
