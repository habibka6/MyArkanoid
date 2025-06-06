#include "WallCollisionSolver.h"

namespace Arkanoid {
    WallCollisionSolver::WallCollisionSolver(float windowWidth, float windowHeight)
        : worldBounds{ 0, 0, windowWidth, windowHeight } {
    }


    void WallCollisionSolver::resolveCollision(Ball& ball, const sf::FloatRect&, float) {
        float radius = ball.getBounds().width * 0.5f;
        auto normal = PhysicsUtils::WallCollision::calculateNormal(ball.getPosition(), radius, worldBounds);

        ball.reflect(normal);
        PhysicsUtils::WallCollision::correctPosition(ball, radius, worldBounds);
    }

    bool WallCollisionSolver::checkWallCollisions(Ball& ball) const {
        float radius = ball.getBounds().width * 0.5f;
        auto pos = ball.getPosition();
        auto vel = ball.getVelocity();

        return (pos.x - radius <= worldBounds.left && vel.x < 0) ||
            (pos.x + radius >= worldBounds.left + worldBounds.width && vel.x > 0) ||
            (pos.y - radius <= worldBounds.top && vel.y < 0);
    }

    bool WallCollisionSolver::isBallLost(const Ball& ball) const {
        return PhysicsUtils::WallCollision::isBallLost(ball, worldBounds.height);
    }
}