#include "BlockCollisionSolver.h"
#include <cmath>
#include <algorithm>
#include "Config.h"

namespace Arkanoid {

    bool BlockCollisionSolver::checkBlockCollision(Ball& ball, BaseBlock& block) const {
        return ball.getBounds().intersects(block.getBounds()) && block.isActive();
    }

void BlockCollisionSolver::resolveBlockCollision(Ball& ball, BaseBlock& block, float deltaTime) {
    float radius = ball.getBounds().width * 0.5f;
    auto [normal, collisionTime] = sweepCollision(
        ball.getPosition(), ball.getVelocity(), radius, block.getBounds(), deltaTime);

    if (normal.x == 0 && normal.y == 0) {
        normal = calculateNormal(ball.getPosition(), block.getBounds());
    }

    if (collisionTime > 0) {
        ball.setPosition(ball.getPosition() + ball.getVelocity() * collisionTime);
        ball.reflect(normal);
    } else {
        ball.reflect(normal);
        correctPosition(ball, block.getBounds(), normal);
    }
    ensureMinimumVerticalVelocity(ball);
} 

    BlockCollisionSolver::CollisionSide BlockCollisionSolver::determineCollisionSide(const sf::Vector2f& ballPos, const sf::FloatRect& blockBounds) {
        sf::Vector2f blockCenter(blockBounds.left + blockBounds.width / 2, blockBounds.top + blockBounds.height / 2);
        sf::Vector2f ballToCenter = blockCenter - ballPos;

        float widthRatio = std::abs(ballToCenter.x) / (blockBounds.width / 2);
        float heightRatio = std::abs(ballToCenter.y) / (blockBounds.height / 2);

        return (widthRatio > heightRatio) ?
            (ballToCenter.x > 0 ? CollisionSide::Left : CollisionSide::Right) :
            (ballToCenter.y > 0 ? CollisionSide::Top : CollisionSide::Bottom);
    }

    sf::Vector2f BlockCollisionSolver::calculateNormal(const sf::Vector2f& ballPos, const sf::FloatRect& blockBounds) {
        switch (determineCollisionSide(ballPos, blockBounds)) {
        case CollisionSide::Top: return { 0, -1 };
        case CollisionSide::Bottom: return { 0, 1 };
        case CollisionSide::Left: return { -1, 0 };
        case CollisionSide::Right: return { 1, 0 };
        default: return { 0, -1 };
        }
    }

    void BlockCollisionSolver::correctPosition(Ball& ball, const sf::FloatRect& blockBounds,
        const sf::Vector2f& normal, float separation) {
        auto ballBounds = ball.getBounds();
        auto ballCenter = ball.getPosition();
        sf::Vector2f correction(0, 0);

        if (std::abs(normal.x) > std::abs(normal.y)) {
            correction.x = normal.x > 0 ?
                (blockBounds.left + blockBounds.width - (ballCenter.x - ballBounds.width / 2) + separation) :
                (blockBounds.left - (ballCenter.x + ballBounds.width / 2) - separation);
        }
        else {
            correction.y = normal.y > 0 ?
                (blockBounds.top + blockBounds.height - (ballCenter.y - ballBounds.height / 2) + separation) :
                (blockBounds.top - (ballCenter.y + ballBounds.height / 2) - separation);
        }
        ball.move(correction);
    }

    void BlockCollisionSolver::ensureMinimumVerticalVelocity(Ball& ball, float minRatio) {
        auto velocity = ball.getVelocity();
        float speed = std::hypot(velocity.x, velocity.y);
        float minVertical = speed * minRatio;
        float minHorizontal = speed * minRatio;

        if (std::abs(velocity.y) < minVertical) {
            velocity.y = (velocity.y < 0) ? -minVertical : minVertical;
        }

        if (std::abs(velocity.x) < minHorizontal) {
            velocity.x = (velocity.x < 0) ? -minHorizontal : minHorizontal;
        }

        float newSpeed = std::hypot(velocity.x, velocity.y);
        if (newSpeed > 0.0f) {
            velocity.x *= speed / newSpeed;
            velocity.y *= speed / newSpeed;
        }

        ball.setVelocity(velocity);
    }



    std::pair<sf::Vector2f, float> BlockCollisionSolver::sweepCollision(
        const sf::Vector2f& startPos, const sf::Vector2f& velocity, float radius,
        const sf::FloatRect& blockBounds, float deltaTime) {

        sf::FloatRect expanded = blockBounds;
        expanded.left -= radius;
        expanded.top -= radius;
        expanded.width += 2 * radius;
        expanded.height += 2 * radius;

        const float epsilon = 1e-6f;
        sf::Vector2f safeVel = velocity;
        if (std::abs(safeVel.x) < epsilon) safeVel.x = epsilon;
        if (std::abs(safeVel.y) < epsilon) safeVel.y = epsilon;

        float tNearX = (expanded.left - startPos.x) / safeVel.x;
        float tFarX = (expanded.left + expanded.width - startPos.x) / safeVel.x;
        float tNearY = (expanded.top - startPos.y) / safeVel.y;
        float tFarY = (expanded.top + expanded.height - startPos.y) / safeVel.y;

        if (tNearX > tFarX) std::swap(tNearX, tFarX);
        if (tNearY > tFarY) std::swap(tNearY, tFarY);

        float tNear = std::max(tNearX, tNearY);
        float tFar = std::min(tFarX, tFarY);

        if (tNear > tFar || tFar < 0 || tNear > deltaTime) {
            return { {0,0}, 0.0f };
        }

        sf::Vector2f normal(0, 0);
        if (tNearX > tNearY) {
            normal.x = (velocity.x > 0) ? -1.0f : 1.0f;
        }
        else {
            normal.y = (velocity.y > 0) ? -1.0f : 1.0f;
        }

        return { normal, std::max(0.0f, tNear) };
    }

}
