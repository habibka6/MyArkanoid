#include "Physics.h"
#include <algorithm>

void Physics::checkPaddleCollision(Ball& ball, const Paddle& paddle, sf::Sound& sound) {
    if (ball.getBounds().intersects(paddle.getGlobalBounds())) {
        // 1. ������ ���� �� ����� �����
        float paddleCenterX = paddle.getPosition().x + paddle.getSize().x / 2;
        float hitPos = (ball.getPosition().x - paddleCenterX) / (paddle.getSize().x / 2);
        hitPos = std::clamp(hitPos, -1.0f, 1.0f);
        float angle = hitPos * 70.0f * (3.14159f / 180.0f);

        // 2. ���� �������� ���������
        sf::Vector2f paddleVel = paddle.getVelocity();
        float speedBoost = std::abs(paddleVel.x) / paddle.getSpeed(); // 0..1

        // 3. ��������������� �������� ����
        ball.setVelocity(
            600.f * sin(angle) + paddleVel.x * 0.7f, // �������������� �������
            -600.f * cos(angle) * (1.0f + speedBoost * 0.5f) // ������������ ���������
        );

        sound.play();
    }
}

void Physics::checkBlockCollisions(Ball& ball, std::vector<Block>& blocks, int& score, sf::Sound& sound) {
    bool collisionFound = false;

    for (auto it = blocks.begin(); it != blocks.end(); ) {
        if (!collisionFound && !it->isDestroyed() && ball.getBounds().intersects(it->getBounds())) {
            sf::FloatRect blockBounds = it->getBounds();
            sf::FloatRect ballBounds = ball.getBounds();

            // ���� ����������� ��������
            sf::Vector2f ballVel = ball.getVelocity();
            float overlapLeft = (ballVel.x > 0) ? (ballBounds.left + ballBounds.width - blockBounds.left) : 0;
            float overlapRight = (ballVel.x < 0) ? (blockBounds.left + blockBounds.width - ballBounds.left) : 0;
            float overlapTop = (ballVel.y > 0) ? (ballBounds.top + ballBounds.height - blockBounds.top) : 0;
            float overlapBottom = (ballVel.y < 0) ? (blockBounds.top + blockBounds.height - ballBounds.top) : 0;

            // ������� ����������� ���������� ����������
            float minOverlap = std::numeric_limits<float>::max();
            sf::Vector2f normal;

            if (overlapLeft > 0 && overlapLeft < minOverlap) { minOverlap = overlapLeft; normal = { -1, 0 }; }
            if (overlapRight > 0 && overlapRight < minOverlap) { minOverlap = overlapRight; normal = { 1, 0 }; }
            if (overlapTop > 0 && overlapTop < minOverlap) { minOverlap = overlapTop; normal = { 0, -1 }; }
            if (overlapBottom > 0 && overlapBottom < minOverlap) { minOverlap = overlapBottom; normal = { 0, 1 }; }

            // ��������� ������� (100% ����������)
            ball.getSprite().move(normal * minOverlap * 1.0f);

            // ������
            ball.reflect(normal);
            sound.play();

            // ���� �����
            if (it->hit()) {
                score += it->getPoints();
                it = blocks.erase(it);
            }
            else {
                ++it;
            }

            collisionFound = true;
            break; // ��������� ����
        }
        else {
            ++it;
        }
    }
}

void Physics::checkWallCollisions(Ball& ball, const sf::RenderWindow& window) {
    // ������������ ������ ��������� �� ����
    sf::Vector2f ballPos = ball.getPosition();
    float ballRadius = ball.getBounds().width / 2;

    if (ballPos.x - ballRadius <= 0) {
        ball.reflect({ 1, 0 });
    }
    else if (ballPos.x + ballRadius >= window.getSize().x) {
        ball.reflect({ -1, 0 });
    }
    if (ballPos.y - ballRadius <= 0) {
        ball.reflect({ 0, 1 });
    }
}

void Physics::checkDeathZone(Ball& ball, int& lives, bool& gameOver, const sf::RenderWindow& window, sf::Sound& sound) {
    // ������������ ������ ������ ����
    if (ball.getPosition().y + ball.getBounds().height / 2 >= window.getSize().y) {
        lives--;
        if (lives <= 0) {
            gameOver = true;
        }
        else {
            ball.reset(window.getSize().x / 2, window.getSize().y - 150);
        }
        sound.play();
    }
}

