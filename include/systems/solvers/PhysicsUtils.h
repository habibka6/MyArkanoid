#pragma once
#include "Ball.h"
#include "Paddle.h"
#include "BaseBlock.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

namespace Arkanoid {
    namespace PhysicsUtils {

        // Общие утилиты
        namespace Common {
            inline float calculateSpeed(const sf::Vector2f& velocity) {
                return std::hypot(velocity.x, velocity.y);
            }

            inline void normalizeVelocity(sf::Vector2f& velocity, float targetSpeed) {
                float speed = calculateSpeed(velocity);
                if (speed > 0) {
                    velocity *= (targetSpeed / speed);
                }
            }

            inline void ensureMinimumVerticalVelocity(Ball& ball, float minRatio = Config::Physics::MIN_VERTICAL_RATIO) {
                auto velocity = ball.getVelocity();
                float speed = calculateSpeed(velocity);
                float minVertical = speed * minRatio;

                if (std::abs(velocity.y) < minVertical) {
                    velocity.y = (velocity.y < 0) ? -minVertical : minVertical;
                    float newHorizontal = std::sqrt(std::max(0.0f, speed * speed - velocity.y * velocity.y));
                    velocity.x = (velocity.x < 0) ? -newHorizontal : newHorizontal;
                    ball.setVelocity(velocity);
                }
            }
        }

        // Столкновения со стенами
        namespace WallCollision {
            inline sf::Vector2f calculateNormal(const sf::Vector2f& ballPos, float radius, const sf::FloatRect& worldBounds) {
                sf::Vector2f normal(0, 0);

                if (ballPos.x - radius <= worldBounds.left) normal.x = 1.0f;
                else if (ballPos.x + radius >= worldBounds.left + worldBounds.width) normal.x = -1.0f;

                if (ballPos.y - radius <= worldBounds.top) normal.y = 1.0f;

                if (normal.x != 0 && normal.y != 0) {
                    float len = std::sqrt(normal.x * normal.x + normal.y * normal.y);
                    normal.x /= len;
                    normal.y /= len;
                }

                return normal;
            }

            inline void correctPosition(Ball& ball, float radius, const sf::FloatRect& worldBounds) {
                auto pos = ball.getPosition();

                if (pos.x - radius < worldBounds.left) pos.x = worldBounds.left + radius;
                else if (pos.x + radius > worldBounds.left + worldBounds.width) pos.x = worldBounds.left + worldBounds.width - radius;

                if (pos.y - radius < worldBounds.top) pos.y = worldBounds.top + radius;

                ball.setPosition(pos);
            }

            inline bool isBallLost(const Ball& ball, float worldHeight) {
                return ball.getPosition().y > worldHeight + 50;
            }
        }

        // Столкновения с блоками
        namespace BlockCollision {
            enum class CollisionSide { Top, Bottom, Left, Right };

            inline CollisionSide determineCollisionSide(const sf::Vector2f& ballPos, const sf::FloatRect& blockBounds) {
                sf::Vector2f blockCenter(blockBounds.left + blockBounds.width / 2, blockBounds.top + blockBounds.height / 2);
                sf::Vector2f ballToCenter = blockCenter - ballPos;

                float widthRatio = std::abs(ballToCenter.x) / (blockBounds.width / 2);
                float heightRatio = std::abs(ballToCenter.y) / (blockBounds.height / 2);

                return (widthRatio > heightRatio) ?
                    (ballToCenter.x > 0 ? CollisionSide::Left : CollisionSide::Right) :
                    (ballToCenter.y > 0 ? CollisionSide::Top : CollisionSide::Bottom);
            }

            inline sf::Vector2f calculateNormal(const sf::Vector2f& ballPos, const sf::FloatRect& blockBounds) {
                switch (determineCollisionSide(ballPos, blockBounds)) {
                case CollisionSide::Top: return { 0, -1 };
                case CollisionSide::Bottom: return { 0, 1 };
                case CollisionSide::Left: return { -1, 0 };
                case CollisionSide::Right: return { 1, 0 };
                default: return { 0, -1 };
                }
            }

            inline void correctPosition(Ball& ball, const sf::FloatRect& blockBounds,
                const sf::Vector2f& normal, float separation = Config::Physics::DEFAULT_SEPARATION) {
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

            inline std::pair<sf::Vector2f, float> sweepCollision(
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


        // Столкновения с платформой
        namespace PaddleCollision {
            inline void calculateAndApplyPaddleReflection(Ball& ball, const Paddle& paddle)  {
                float halfW = paddle.getSize().x * 0.5f;
                float paddleCX = paddle.getPosition().x + halfW;
                float hitOffset = (ball.getPosition().x - paddleCX) / halfW;
                hitOffset = std::clamp(hitOffset, -1.0f, 1.0f);

                const float MIN_ANGLE_DEG = 15.0f;  
                const float MAX_ANGLE_DEG = 75.0f;  

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
            inline sf::Vector2f calculateReflectionNormal(const sf::Vector2f& ballPos, const sf::FloatRect& paddleBounds)  {

                float relativeX = (ballPos.x - paddleBounds.left) / paddleBounds.width;
                relativeX = std::clamp(relativeX, 0.0f, 1.0f);
                float maxAngle = 60.0f; 
                float angle = (relativeX - 0.5f) * maxAngle * 2.0f;
                float angleRad = angle * Config::Physics::DEG_TO_RAD;

                return sf::Vector2f(std::sin(angleRad), -std::abs(std::cos(angleRad)));
            }

            inline void applyPaddleInfluence(Ball& ball, const Paddle& paddle, float influence = Config::Physics::PADDLE_INFLUENCE) {
                auto vel = ball.getVelocity();
                vel.x += paddle.getVelocity().x * influence;
                ball.setVelocity(vel);
            }

            inline void correctPosition(Ball& ball, const sf::FloatRect& paddleBounds) {
                auto ballBounds = ball.getBounds();
                if (ballBounds.top + ballBounds.height > paddleBounds.top) {
                    ball.setPosition({ ball.getPosition().x, paddleBounds.top - ballBounds.height / 2 - 2.0f });
                }
            }
        }

    } 
} 