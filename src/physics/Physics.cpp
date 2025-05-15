#include "Physics.h"
#include <algorithm>
#include <iostream> // Для отладки
#include <random>

std::pair<sf::Vector2f, float> Physics::sweepAABB(
    const sf::Vector2f& startPos,
    const sf::Vector2f& velocity,
    float dt,
    const sf::FloatRect& box,
    float radius
) {
    sf::Vector2f endPos = startPos + velocity * dt;
    sf::FloatRect expanded = box;

    // Расширяем AABB для учета радиуса мяча
    expanded.left -= radius;
    expanded.top -= radius;
    expanded.width += 2.f * radius;
    expanded.height += 2.f * radius;

    float tEntry = 0.f, tExit = 1.f;

    // Проверка столкновений по осям X и Y
    for (int axis = 0; axis < 2; ++axis) {
        float s = (axis == 0) ? startPos.x : startPos.y;
        float e = (axis == 0) ? endPos.x : endPos.y;
        float min = (axis == 0) ? expanded.left : expanded.top;
        float max = (axis == 0) ? (expanded.left + expanded.width)
            : (expanded.top + expanded.height);
        float d = e - s;

        if (d == 0.f) {
            if (s < min || s > max) return { {0, 0}, 0.f };
        }
        else {
            float t1 = (min - s) / d;
            float t2 = (max - s) / d;
            if (t1 > t2) std::swap(t1, t2);
            tEntry = std::max(tEntry, t1);
            tExit = std::min(tExit, t2);
            if (tEntry > tExit || tExit < 0.f || tEntry > 1.f) {
                return { {0, 0}, 0.f };
            }
        }
    }

    // Точка первого контакта
    sf::Vector2f hitPoint = startPos + velocity * dt * tEntry;
    const float eps = 1e-2f; // Погрешность для угловых проверок
    sf::Vector2f normal{ 0, 0 };

    // Проверка на угловое столкновение
    bool nearLeft = (std::fabs(hitPoint.x - expanded.left) < eps);
    bool nearRight = (std::fabs(hitPoint.x - (expanded.left + expanded.width)) < eps);
    bool nearTop = (std::fabs(hitPoint.y - expanded.top) < eps);
    bool nearBottom = (std::fabs(hitPoint.y - (expanded.top + expanded.height)) < eps);

    // Если близко к двум граням одновременно - это угол
    if ((nearLeft || nearRight) && (nearTop || nearBottom)) {
        sf::Vector2f combinedNormal(0, 0);
        if (nearLeft) combinedNormal.x = -1;
        if (nearRight) combinedNormal.x = 1;
        if (nearTop) combinedNormal.y = -1;
        if (nearBottom) combinedNormal.y = 1;

        // Нормализация
        float len = std::hypot(combinedNormal.x, combinedNormal.y);
        if (len != 0) {
            normal = combinedNormal / len;
        }
    }
    else {
        // Обычные столкновения с гранями
        if (nearLeft)          normal = { -1, 0 };
        else if (nearRight)    normal = { 1, 0 };
        else if (nearTop)      normal = { 0, -1 };
        else if (nearBottom)   normal = { 0, 1 };
    }

    return { normal, tEntry };
}

void Physics::checkPaddleCollision(Ball& ball, const Paddle& paddle, sf::Sound& sound) {
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


void Physics::checkBlockCollisions(
    Ball& ball,
    std::vector<std::unique_ptr<BaseBlock>>& blocks,
    int& score,
    float dt,
    sf::Sound& blockSound,
    sf::Sound& rockSound
) {
    const sf::Vector2f startPos = ball.getPosition();
    const sf::Vector2f vel = ball.getVelocity();
    const float radius = ball.getBounds().width * 0.5f;

    // Используем итераторы для безопасного удаления блоков
    for (auto it = blocks.begin(); it != blocks.end();) {
        auto& blk = *it;

        if (blk->isDestroyed()) {
            ++it;
            continue;
        }

        // Проверка столкновения с использованием алгоритма Swept AABB
        auto [normal, tEntry] = sweepAABB(startPos, vel, dt, blk->getBounds(), radius);

        if (normal == sf::Vector2f{ 0.f, 0.f }) {
            ++it;
            continue;
        }

        // --- Коррекция позиции и отскок ---
        const sf::Vector2f correction = vel * dt * tEntry + normal * 1.0f;
        ball.getSprite().setPosition(startPos + correction);
        ball.reflect(normal);

        // --- Динамическое изменение скорости ---
        static float speedFactor = 1.0f;
        speedFactor = std::min(speedFactor + SPEED_GROWTH, MAX_SPEED_FACTOR);

        sf::Vector2f newVel = ball.getVelocity() * speedFactor;
        ball.setVelocity(newVel.x, newVel.y);

        // Коррекция минимальной вертикальной скорости
        const float speed = ball.getSpeed() * speedFactor;
        const float minVy = speed * MIN_VERT_RATIO;
        newVel = ball.getVelocity();

        if (std::abs(newVel.y) < minVy) {
            newVel.y = (newVel.y < 0.f ? -minVy : minVy);
            const float len = std::hypot(newVel.x, newVel.y);
            newVel = (newVel / len) * speed;
            ball.setVelocity(newVel.x, newVel.y);
        }

        // --- Воспроизведение звука ---
        if (blk->IsRock()) {
            rockSound.play();
        }
        else {
            blockSound.play();
        }

        // --- Уничтожение блока ---
        if (blk->hit()) {
            score += blk->getPoints();
            it = blocks.erase(it); // Безопасное удаление через итератор
        }
        else {
            ++it;
        }

        break; // Обрабатываем только одно столкновение за шаг
    }
}
void Physics::checkWallCollisions(Ball& ball, const sf::RenderWindow& window)
{
    sf::Vector2f pos = ball.getPosition();
    float radius = ball.getBounds().width * 0.5f;
    sf::Vector2u size = window.getSize();
    sf::Vector2f vel = ball.getVelocity();

    // Для рандома угла
    static std::mt19937            gen{ std::random_device{}() };
    static std::uniform_real_distribution <float> noise{ -WALL_ANGLE_VARIAT, WALL_ANGLE_VARIAT };

    bool bounced = false;
    sf::Vector2f normal{};

    // Левая стена
    if (pos.x - radius <= 0.f && vel.x < 0.f) {
        normal = { 1.f, 0.f };
        bounced = true;
    }
    // Правая стена
    else if (pos.x + radius >= (float)size.x && vel.x > 0.f) {
        normal = { -1.f, 0.f };
        bounced = true;
    }
    // Верхняя стена
    if (pos.y - radius <= 0.f && vel.y < 0.f) {
        normal = { 0.f, 1.f };
        bounced = true;
    }
    
    if (!bounced)
        return;
    // 1) Отражаем вектор
    //    v' = v - 2*(v·n)*n
    float dot = vel.x * normal.x + vel.y * normal.y;
    sf::Vector2f refl = {
        vel.x - 2 * dot * normal.x,
        vel.y - 2 * dot * normal.y
    };

    // 2) Деформируем угол чуть-чуть шумом
    float angle = std::atan2(refl.y, refl.x);
    angle += noise(gen);

    // 3) Понижаем скорость (дампинг)
    float speed = std::hypot(refl.x, refl.y) * WALL_DAMPING;

    // 4) Новая скорость
    sf::Vector2f newVel = {
        std::cos(angle) * speed,
        std::sin(angle) * speed
    };
    ball.setVelocity(newVel.x, newVel.y);

    // 5) Коррекция позиции, чтобы не «залипать» в стене
    //    ставим мяч чуть дальше от стены
    if (normal.x != 0.f)
        ball.getSprite().move(normal.x * (radius + 0.5f), 0.f);
    else
        ball.getSprite().move(0.f, normal.y * (radius + 0.5f));
}

void Physics::checkDeathZone(Ball& ball, int& lives, bool& gameOver,
    const sf::RenderWindow& window, sf::Sound& sound)
{
    if (ball.getPosition().y + ball.getBounds().height / 2 >= window.getSize().y) {
        lives--;
        sound.play();

        if (lives <= 0) {
            gameOver = true;
        }
        else {
            // при GameEngine нужно установЛЕНО waitingLaunch = true
            // сам Ball просто оставляем внизу resetBallPosition()
        }
    }
}