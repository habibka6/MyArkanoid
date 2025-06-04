#include "BlockCollisionSolver.h"

namespace Arkanoid {
    bool BlockCollisionSolver::checkCollision(Ball& ball, const sf::FloatRect& bounds) const {
        return ball.getBounds().intersects(bounds);
    }

    void BlockCollisionSolver::resolveCollision(Ball& ball, const sf::FloatRect& bounds, float deltaTime) {
        auto normal = PhysicsUtils::BlockCollision::calculateNormal(ball.getPosition(), bounds);
        ball.reflect(normal);
        PhysicsUtils::BlockCollision::correctPosition(ball, bounds, normal);
    }

    bool BlockCollisionSolver::checkBlockCollision(Ball& ball, BaseBlock& block) const {
        return ball.getBounds().intersects(block.getBounds()) && block.isActive();
    }

    void BlockCollisionSolver::resolveBlockCollision(Ball& ball, BaseBlock& block, float deltaTime) {
        float radius = ball.getBounds().width * 0.5f;
        auto [normal, collisionTime] = PhysicsUtils::BlockCollision::sweepCollision(
            ball.getPosition(), ball.getVelocity(), radius, block.getBounds(), deltaTime);

        if (normal.x == 0 && normal.y == 0) {
            normal = PhysicsUtils::BlockCollision::calculateNormal(ball.getPosition(), block.getBounds());
        }

        if (collisionTime > 0) {
            ball.setPosition(ball.getPosition() + ball.getVelocity() * collisionTime);
        }

        ball.reflect(normal);
        PhysicsUtils::BlockCollision::correctPosition(ball, block.getBounds(), normal);
        PhysicsUtils::Common::ensureMinimumVerticalVelocity(ball);
    }
}