#include "Ball.h"
#include <cmath>
#include <random>

constexpr float PI = 3.14159f;
constexpr float DEG_TO_RAD = PI / 180.0f;

Ball::Ball(float startX, float startY) {
    sprite.setTexture(AssetManager::GetTexture("ball.png"));
    sprite.setScale(1.5f, 1.5f);
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    reset(startX, startY);
}

void Ball::reflect(const sf::Vector2f& normal) {
    // Нормализация скорости после отскока
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    float dot = velocity.x * normal.x + velocity.y * normal.y;
    velocity.x -= 2 * dot * normal.x;
    velocity.y -= 2 * dot * normal.y;

    // Сохраняем исходную скорость
    float newSpeed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (newSpeed > 0) {
        velocity = (velocity / newSpeed) * speed;
    }
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

void Ball::draw(sf::RenderWindow& window) {
    window.draw(sprite);
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