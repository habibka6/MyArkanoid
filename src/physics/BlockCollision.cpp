#include "BlockCollision.h"
#include <algorithm>
#include <iostream> // Для отладки
#include <random>

std::pair<sf::Vector2f, float> BlockCollision::sweepAABB(
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


void BlockCollision::checkBlockCollisions(
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

        // Коррекция позиции и отскок
        const sf::Vector2f correction = vel * dt * tEntry + normal * 1.0f;
        ball.getSprite().setPosition(startPos + correction);
        ball.reflect(normal);

        // Динамическое изменение скорости
        ball.increaseSpeedFactor(SPEED_GROWTH); // Увеличиваем множитель скорости

        // Рассчитываем текущую скорость с учетом множителя
        const float currentSpeed = ball.getSpeed() * ball.getSpeedFactor();

        // Получаем текущий вектор скорости и нормализуем его
        sf::Vector2f newVel = ball.getVelocity();
        const float velocityLength = std::hypot(newVel.x, newVel.y);

        // Если скорость нулевая (защита от деления на ноль)
        if (velocityLength > 0) {
            newVel = (newVel / velocityLength) * currentSpeed;
            ball.setVelocity(newVel.x, newVel.y);
        }

        // Коррекция минимальной вертикальной скорости 
        const float minVy = currentSpeed * MIN_VERT_RATIO; // Используем currentSpeed
        newVel = ball.getVelocity();

        // Если вертикальная скорость слишком мала
        if (std::abs(newVel.y) < minVy) {
            // Устанавливаем минимальную вертикальную скорость с сохранением направления
            newVel.y = (newVel.y < 0.f ? -minVy : minVy);

            // Нормализуем вектор и масштабируем до текущей скорости
            const float len = std::hypot(newVel.x, newVel.y);
            if (len > 0) {
                newVel = (newVel / len) * currentSpeed;
                ball.setVelocity(newVel.x, newVel.y);
            }
        }

        // Воспроизведение звука 
        if (blk->IsRock()) {
            rockSound.play();
        }
        else {
            blockSound.play();
        }

        //Уничтожение блока
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
