#include "PaddleCollision.h"

void PaddleCollision::checkPaddleCollision(Ball& ball, const Paddle& paddle, sf::Sound& sound) {
    // �������� �����������
    if (!ball.getBounds().intersects(paddle.getGlobalBounds()))
        return;

    // hitOffset � ��������� [-1..1]
    float halfW = paddle.getSize().x * 0.5f;
    float paddleCX = paddle.getPosition().x + halfW;
    float hitOffset = (ball.getPosition().x - paddleCX) / halfW;
    hitOffset = std::clamp(hitOffset, -1.f, 1.f);

    // ������������� ���� ����� MIN_ANGLE � MAX_ANGLE
    float t = std::abs(hitOffset);
    float angleDeg = MIN_ANGLE_DEG + (MAX_ANGLE_DEG - MIN_ANGLE_DEG) * t;
    angleDeg *= (hitOffset < 0 ? -1.f : 1.f);

    // ��������� � �������
    float angleRad = angleDeg * (3.1415926f / 180.f);

    // ����� �������� ����������� ������
    float speed = ball.getSpeed();
    sf::Vector2f newVel;
    newVel.x = std::sin(angleRad) * speed;
    newVel.y = -std::cos(angleRad) * speed;

    // ��������� ���� �������� ���������
    newVel.x += paddle.getVelocity().x * PADDLE_INFLUENCE;

    // ������������� ����� ��������
    ball.setVelocity(newVel.x, newVel.y);

    // ��������� ��� ��� ����������, ����� �� �����
    ball.getSprite().move(0.f, -2.f);

    // ����������� ����
    sound.play();
}
