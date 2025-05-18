#include "PaddleCollision.h"

void PaddleCollision::checkPaddleCollision(Ball& ball, const Paddle& paddle, sf::Sound& sound) {
    // Проверка пересечения
    if (!ball.getBounds().intersects(paddle.getGlobalBounds()))
        return;

    // hitOffset в диапазоне [-1..1]
    float halfW = paddle.getSize().x * 0.5f;
    float paddleCX = paddle.getPosition().x + halfW;
    float hitOffset = (ball.getPosition().x - paddleCX) / halfW;
    hitOffset = std::clamp(hitOffset, -1.f, 1.f);

    // Интерполируем угол между MIN_ANGLE и MAX_ANGLE
    float t = std::abs(hitOffset);
    float angleDeg = MIN_ANGLE_DEG + (MAX_ANGLE_DEG - MIN_ANGLE_DEG) * t;
    angleDeg *= (hitOffset < 0 ? -1.f : 1.f);

    // Переводим в радианы
    float angleRad = angleDeg * (3.1415926f / 180.f);

    // Новая скорость постоянного модуля
    float speed = ball.getSpeed();
    sf::Vector2f newVel;
    newVel.x = std::sin(angleRad) * speed;
    newVel.y = -std::cos(angleRad) * speed;

    // Добавляем долю скорости платформы
    newVel.x += paddle.getVelocity().x * PADDLE_INFLUENCE;

    // Устанавливаем новую скорость
    ball.setVelocity(newVel.x, newVel.y);

    // Поднимаем мяч над платформой, чтобы не залип
    ball.getSprite().move(0.f, -2.f);

    // Проигрываем звук
    sound.play();
}
