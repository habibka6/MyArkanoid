#include "WallCollision.h"

void WallCollision::checkWallCollisions(Ball& ball, const sf::RenderWindow& window)
{
    sf::Vector2f pos = ball.getPosition();
    float radius = ball.getBounds().width * 0.5f;
    sf::Vector2u size = window.getSize();
    sf::Vector2f vel = ball.getVelocity();

    // ��� ������� ����
    static std::mt19937            gen{ std::random_device{}() };
    static std::uniform_real_distribution <float> noise{ -WALL_ANGLE_VARIAT, WALL_ANGLE_VARIAT };

    bool bounced = false;
    sf::Vector2f normal{};

    // ����� �����
    if (pos.x - radius <= 0.f && vel.x < 0.f) {
        normal = { 1.f, 0.f };
        bounced = true;
    }
    // ������ �����
    else if (pos.x + radius >= (float)size.x && vel.x > 0.f) {
        normal = { -1.f, 0.f };
        bounced = true;
    }
    // ������� �����
    if (pos.y - radius <= 0.f && vel.y < 0.f) {
        normal = { 0.f, 1.f };
        bounced = true;
    }

    if (!bounced)
        return;
    // 1) �������� ������
    //    v' = v - 2*(v�n)*n
    float dot = vel.x * normal.x + vel.y * normal.y;
    sf::Vector2f refl = {
        vel.x - 2 * dot * normal.x,
        vel.y - 2 * dot * normal.y
    };

    // 2) ����������� ���� ����-���� �����
    float angle = std::atan2(refl.y, refl.x);
    angle += noise(gen);

    // 3) �������� �������� (�������)
    float speed = std::hypot(refl.x, refl.y) * WALL_DAMPING;

    // 4) ����� ��������
    sf::Vector2f newVel = {
        std::cos(angle) * speed,
        std::sin(angle) * speed
    };
    ball.setVelocity(newVel.x, newVel.y);

    // 5) ��������� �������, ����� �� ���������� � �����
    //    ������ ��� ���� ������ �� �����
    if (normal.x != 0.f)
        ball.getSprite().move(normal.x * (radius + 0.5f), 0.f);
    else
        ball.getSprite().move(0.f, normal.y * (radius + 0.5f));
}