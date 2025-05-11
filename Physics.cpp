#include "Physics.h"
#include <algorithm>

void Physics::checkPaddleCollision(Ball& ball, const Paddle& paddle, sf::Sound& sound) {
    if (ball.getBounds().intersects(paddle.getGlobalBounds())) {
        // 1. Расчет угла от точки удара
        float paddleCenterX = paddle.getPosition().x + paddle.getSize().x / 2;
        float hitPos = (ball.getPosition().x - paddleCenterX) / (paddle.getSize().x / 2);
        hitPos = std::clamp(hitPos, -1.0f, 1.0f);
        float angle = hitPos * 70.0f * (3.14159f / 180.0f);

        // 2. Учет скорости платформы
        sf::Vector2f paddleVel = paddle.getVelocity();
        float speedBoost = std::abs(paddleVel.x) / paddle.getSpeed(); // 0..1

        // 3. Комбинированная скорость мяча
        ball.setVelocity(
            600.f * sin(angle) + paddleVel.x * 0.7f, // Горизонтальный импульс
            -600.f * cos(angle) * (1.0f + speedBoost * 0.5f) // Вертикальный усилитель
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

            // Учет направления скорости
            sf::Vector2f ballVel = ball.getVelocity();
            float overlapLeft = (ballVel.x > 0) ? (ballBounds.left + ballBounds.width - blockBounds.left) : 0;
            float overlapRight = (ballVel.x < 0) ? (blockBounds.left + blockBounds.width - ballBounds.left) : 0;
            float overlapTop = (ballVel.y > 0) ? (ballBounds.top + ballBounds.height - blockBounds.top) : 0;
            float overlapBottom = (ballVel.y < 0) ? (blockBounds.top + blockBounds.height - ballBounds.top) : 0;

            // Находим минимальное актуальное перекрытие
            float minOverlap = std::numeric_limits<float>::max();
            sf::Vector2f normal;

            if (overlapLeft > 0 && overlapLeft < minOverlap) { minOverlap = overlapLeft; normal = { -1, 0 }; }
            if (overlapRight > 0 && overlapRight < minOverlap) { minOverlap = overlapRight; normal = { 1, 0 }; }
            if (overlapTop > 0 && overlapTop < minOverlap) { minOverlap = overlapTop; normal = { 0, -1 }; }
            if (overlapBottom > 0 && overlapBottom < minOverlap) { minOverlap = overlapBottom; normal = { 0, 1 }; }

            // Коррекция позиции (100% перекрытия)
            ball.getSprite().move(normal * minOverlap * 1.0f);

            // Отскок
            ball.reflect(normal);
            sound.play();

            // Урон блоку
            if (it->hit()) {
                score += it->getPoints();
                it = blocks.erase(it);
            }
            else {
                ++it;
            }

            collisionFound = true;
            break; // Прерываем цикл
        }
        else {
            ++it;
        }
    }
}

void Physics::checkWallCollisions(Ball& ball, const sf::RenderWindow& window) {
    // Оригинальная логика отражения от стен
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
    // Оригинальная логика потери мяча
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

