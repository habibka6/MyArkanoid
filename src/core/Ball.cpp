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
    // Сохраняем текущую скорость с учетом множителя
    float currentSpeed = std::hypot(velocity.x, velocity.y);

    // Отражение вектора скорости
    float dot = velocity.x * normal.x + velocity.y * normal.y;
    velocity.x -= 2 * dot * normal.x;
    velocity.y -= 2 * dot * normal.y;

    // Нормализация и сохранение скорости (без изменения множителя)
    velocity = velocity / std::hypot(velocity.x, velocity.y) * currentSpeed;
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
        // Учитываем множитель скорости
        velocity = (velocity / currentSpeed) * baseSpeed * speedFactor;
    }
}
void Ball::increaseSpeedFactor(float value) {
    speedFactor = std::min(speedFactor + value, MAX_SPEED_FACTOR);
    // Плавное обновление скорости
    float currentSpeed = std::hypot(velocity.x, velocity.y);
    velocity = (velocity / currentSpeed) * baseSpeed * speedFactor;
}

void Ball::update(float dt) {
    const int microSteps = 8; // Было 4
    float stepDt = dt / microSteps;

    for (int i = 0; i < microSteps; ++i) {
        m_prevPosition = sprite.getPosition();
        sprite.move(velocity * stepDt);
    }
}

void Ball::draw(sf::RenderWindow& window, float alpha) const {
    // Использовать временный спрайт для интерполяции
    sf::Sprite tempSprite = sprite;
    sf::Vector2f renderPos = m_prevPosition * (1 - alpha) + tempSprite.getPosition() * alpha;
    tempSprite.setPosition(renderPos);
    window.draw(tempSprite);
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