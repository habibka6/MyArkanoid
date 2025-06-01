#include "PaddleCollisionSolver.h"
#include "SoundManager.h"

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

        SoundManager::getInstance().playSound(SoundType::PaddleHit);
        ball.increaseSpeedFactor(0.02f);
    }
}