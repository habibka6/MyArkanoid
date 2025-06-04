#include "PaddleCollisionSolver.h"

namespace Arkanoid {
    bool PaddleCollisionSolver::checkCollision(Ball& ball, const sf::FloatRect& bounds) const {
        return ball.getBounds().intersects(bounds);
    }

    void PaddleCollisionSolver::resolveCollision(Ball& ball, const sf::FloatRect& bounds, float) {
        auto normal = PhysicsUtils::PaddleCollision::calculateReflectionNormal(ball.getPosition(), bounds);
        ball.reflect(normal);
        PhysicsUtils::PaddleCollision::correctPosition(ball, bounds);
    }

    bool PaddleCollisionSolver::checkPaddleCollision(Ball& ball, Paddle& paddle) const {
        return ball.getBounds().intersects(paddle.getBounds()) && ball.getVelocity().y > 0;
    }

    void PaddleCollisionSolver::resolvePaddleCollision(Ball& ball, Paddle& paddle, float) {
        PhysicsUtils::PaddleCollision::calculateAndApplyPaddleReflection(ball, paddle);
        PhysicsUtils::PaddleCollision::applyPaddleInfluence(ball, paddle);
        PhysicsUtils::PaddleCollision::correctPosition(ball, paddle.getBounds());

        ball.increaseSpeedFactor(Config::Ball::SPEED_INCREASE_PER_HIT);
    }
}