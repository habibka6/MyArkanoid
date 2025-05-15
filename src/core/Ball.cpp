#include "Ball.h"
#include <cmath>
#include <random>

constexpr float PI = 3.14159f;
constexpr float DEG_TO_RAD = PI / 180.0f;

Ball::Ball(float startX, float startY) {
    sprite.setTexture(AssetManager::getInstance().GetTexture("ball.png"));
    sprite.setScale(1.5f, 1.5f);
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    reset(startX, startY);
}

void Ball::reflect(const sf::Vector2f& normal) {
    const float speed = std::hypot(velocity.x, velocity.y);
    const float dot = velocity.x * normal.x + velocity.y * normal.y;
    velocity -= 2 * dot * normal;
    velocity = velocity / std::hypot(velocity.x, velocity.y) * speed;
}

void Ball::reset(float x, float y) {
    sprite.setPosition(x, y);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> angleDist(-45.0f, 45.0f);
    float angle = angleDist(gen) * DEG_TO_RAD;

    velocity.x = baseSpeed * sin(angle);
    velocity.y = -baseSpeed * cos(angle);
}

void Ball::correctPosition(const sf::FloatRect& paddleBounds, float dt) {
    sf::FloatRect ballBounds = sprite.getGlobalBounds();
    sf::Vector2f prevPos = sprite.getPosition() - velocity * dt;

    if (paddleBounds.contains(prevPos)) {
        sprite.setPosition(sprite.getPosition().x, paddleBounds.top - ballBounds.height / 2);
    }
}

void Ball::setVelocity(float x, float y) {
    velocity = { x, y };
    float currentSpeed = std::hypot(x, y);
    if (currentSpeed > 0) {
        velocity = (velocity / currentSpeed) * baseSpeed;
    }
}

void Ball::update(float dt) {
    sprite.move(velocity * dt);
}

void Ball::draw(sf::RenderWindow& window, float alpha) {
    sf::Vector2f renderPos = m_prevPosition * (1 - alpha) + sprite.getPosition() * alpha;
    sf::Vector2f originalPos = sprite.getPosition();
    sprite.setPosition(renderPos);
    window.draw(sprite);
    sprite.setPosition(originalPos);
}

sf::FloatRect Ball::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Ball::getPosition() const {
    return sprite.getPosition();
}

sf::Vector2f Ball::getVelocity() const {
    return velocity;
}

float Ball::getSpeed() const {
    return baseSpeed;
}