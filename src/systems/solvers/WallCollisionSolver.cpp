#include "WallCollisionSolver.h"
#include <cmath>

namespace Arkanoid {

    WallCollisionSolver::WallCollisionSolver(float windowWidth, float windowHeight)
        : worldBounds{ 0, 0, windowWidth, windowHeight } {
    }
    void WallCollisionSolver::resolveCollision(Ball& ball, const sf::FloatRect&, float deltaTime) {
        float radius = ball.getBounds().width * 0.5f;
        auto pos = ball.getPosition();
        auto vel = ball.getVelocity();
        sf::Vector2f normal(0, 0);

        constexpr float epsilon = 0.01f;

        if (pos.x - radius < worldBounds.left + epsilon && vel.x < 0) {
            normal.x = 1.0f;
            pos.x = worldBounds.left + radius + epsilon;
        }
        else if (pos.x + radius > worldBounds.left + worldBounds.width - epsilon && vel.x > 0) {
            normal.x = -1.0f;
            pos.x = worldBounds.left + worldBounds.width - radius - epsilon;
        }

        if (pos.y - radius < worldBounds.top + epsilon && vel.y < 0) {
            normal.y = 1.0f;
            pos.y = worldBounds.top + radius + epsilon;
        }

        if (normal.x != 0 || normal.y != 0) {
            ball.reflect(normal);
            ball.setPosition(pos);
        }
    }


    bool WallCollisionSolver::checkWallCollisions(Ball& ball) const {
        float radius = ball.getBounds().width * 0.5f;
        auto pos = ball.getPosition();
        auto vel = ball.getVelocity();

        return (pos.x - radius <= worldBounds.left && vel.x < 0) ||
            (pos.x + radius >= worldBounds.left + worldBounds.width && vel.x > 0) ||
            (pos.y - radius <= worldBounds.top && vel.y < 0);
    }

    

    bool WallCollisionSolver::isBallLost(const Ball& ball, float worldHeight) {
        return ball.getPosition().y > worldHeight + 50;
    }

}
