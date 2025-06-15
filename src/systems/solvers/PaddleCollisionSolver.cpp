#include "PaddleCollisionSolver.h"
#include "Config.h"
#include <cmath>
#include <algorithm>

namespace Arkanoid {

    bool PaddleCollisionSolver::checkPaddleCollision(Ball& ball, Paddle& paddle) const {
        return ball.getBounds().intersects(paddle.getBounds()) && ball.getVelocity().y > 0;
    }

    void PaddleCollisionSolver::resolvePaddleCollision(Ball& ball, Paddle& paddle, float) {
        calculateAndApplyPaddleReflection(ball, paddle);
        applyPaddleInfluence(ball, paddle);
        correctPosition(ball, paddle.getBounds());
        ball.increaseSpeedFactor(Config::Ball::SPEED_INCREASE_PER_HIT);
    }


    void PaddleCollisionSolver::calculateAndApplyPaddleReflection(Ball& ball, const Paddle& paddle) {
        float halfW = paddle.getSize().x * 0.5f;
        float paddleCX = paddle.getPosition().x + halfW;
        float hitOffset = (ball.getPosition().x - paddleCX) / halfW;
        hitOffset = std::clamp(hitOffset, -1.0f, 1.0f);

        const float MIN_ANGLE_DEG = Config::Physics::MIN_PADDLE_ANGLE;
        const float MAX_ANGLE_DEG = Config::Physics::MAX_PADDLE_ANGLE;

        float t = std::abs(hitOffset);
        float angleDeg = MIN_ANGLE_DEG + (MAX_ANGLE_DEG - MIN_ANGLE_DEG) * t;
        angleDeg *= (hitOffset < 0 ? -1.0f : 1.0f);

        float angleRad = angleDeg * (Config::Physics::DEG_TO_RAD);

        float speed = ball.getBaseSpeed() * ball.getSpeedFactor();
        sf::Vector2f newVel;
        newVel.x = std::sin(angleRad) * speed;
        newVel.y = -std::cos(angleRad) * speed;

        ball.setVelocity(newVel);
    }

    sf::Vector2f PaddleCollisionSolver::calculateReflectionNormal(const sf::Vector2f& ballPos, const sf::FloatRect& paddleBounds) {
        float relativeX = (ballPos.x - paddleBounds.left) / paddleBounds.width;
        relativeX = std::clamp(relativeX, 0.0f, 1.0f);
        float maxAngle = 60.0f;
        float angle = (relativeX - 0.5f) * maxAngle * 2.0f;
        float angleRad = angle * Config::Physics::DEG_TO_RAD;

        return sf::Vector2f(std::sin(angleRad), -std::abs(std::cos(angleRad)));
    }

    void PaddleCollisionSolver::applyPaddleInfluence(Ball& ball, const Paddle& paddle, float influence) {
        auto vel = ball.getVelocity();
        vel.x += paddle.getVelocity().x * influence;
        ball.setVelocity(vel);
    }

    void PaddleCollisionSolver::correctPosition(Ball& ball, const sf::FloatRect& paddleBounds) {
        auto ballBounds = ball.getBounds();
        if (ballBounds.top + ballBounds.height > paddleBounds.top) {
            ball.setPosition({ ball.getPosition().x, paddleBounds.top - ballBounds.height / 2 - 2.0f });
        }
    }
}
