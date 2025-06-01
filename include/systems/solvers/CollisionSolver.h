#pragma once
#include <SFML/Graphics.hpp>
#include "Ball.h"

namespace Arkanoid {

    // Базовый интерфейс для решения коллизий (паттерн Strategy)
    class CollisionSolver {
    public:
        virtual ~CollisionSolver() = default;

        virtual bool checkCollision(Ball& ball, const sf::FloatRect& bounds) const = 0;
        virtual void resolveCollision(Ball& ball, const sf::FloatRect& bounds, float deltaTime) = 0;
        

    protected:
        CollisionSolver() = default;
    };

} // namespace Arkanoid
