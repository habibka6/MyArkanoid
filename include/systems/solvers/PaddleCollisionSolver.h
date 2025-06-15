#pragma once
#include "Paddle.h"
#include "Ball.h"
#include <SFML/Graphics.hpp>

namespace Arkanoid {

class PaddleCollisionSolver {
public:
    bool checkPaddleCollision(Ball& ball, Paddle& paddle) const;
    void resolvePaddleCollision(Ball& ball, Paddle& paddle, float deltaTime);

    // --- Методы из PhysicsUtils ---
    static void calculateAndApplyPaddleReflection(Ball& ball, const Paddle& paddle);
    static sf::Vector2f calculateReflectionNormal(const sf::Vector2f& ballPos, const sf::FloatRect& paddleBounds);
    static void applyPaddleInfluence(Ball& ball, const Paddle& paddle, float influence = Config::Physics::PADDLE_INFLUENCE);
    static void correctPosition(Ball& ball, const sf::FloatRect& paddleBounds);
};

}
