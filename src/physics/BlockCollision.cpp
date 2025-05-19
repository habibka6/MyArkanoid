#include "BlockCollision.h"
#include "SpatialGrid.h"
#include <algorithm>
#include <vector>
#include <unordered_set>

std::pair<sf::Vector2f, float> BlockCollision::sweepAABB(
    const sf::Vector2f& startPos,
    const sf::Vector2f& velocity,
    float dt,
    const sf::FloatRect& box,
    float radius
) {
    sf::Vector2f endPos = startPos + velocity * dt;
    sf::FloatRect expanded = box;

    expanded.left -= radius;
    expanded.top -= radius;
    expanded.width += 2 * radius;
    expanded.height += 2 * radius;

    const float epsilon = 1e-5f;
    float velX = (std::abs(velocity.x) < epsilon ? epsilon : velocity.x);
    float velY = (std::abs(velocity.y) < epsilon ? epsilon : velocity.y);

    float tx1 = (expanded.left - startPos.x) / velX;
    float tx2 = (expanded.left + expanded.width - startPos.x) / velX;
    float ty1 = (expanded.top - startPos.y) / velY;
    float ty2 = (expanded.top + expanded.height - startPos.y) / velY;

    float tEntryX = std::min(tx1, tx2);
    float tExitX = std::max(tx1, tx2);
    float tEntryY = std::min(ty1, ty2);
    float tExitY = std::max(ty1, ty2);

    float tEntry = std::max(tEntryX, tEntryY);
    float tExit = std::min(tExitX, tExitY);

    if (tEntry > tExit || tExit < 0 || tEntry > dt) {
        return { sf::Vector2f(0, 0), 0.f };
    }

    sf::Vector2f normal(0.f, 0.f);
    bool isCorner = std::abs(tEntryX - tEntryY) < epsilon;

    // Уточнение нормали для углов
    if (isCorner) {
        float dx = (velocity.x < 0) ? (expanded.left - startPos.x) : (startPos.x - (expanded.left + expanded.width));
        float dy = (velocity.y < 0) ? (expanded.top - startPos.y) : (startPos.y - (expanded.top + expanded.height));
        if (std::abs(dx) < std::abs(dy)) {
            normal.x = (velocity.x < 0) ? 1.f : -1.f;
        }
        else {
            normal.y = (velocity.y < 0) ? 1.f : -1.f;
        }
    }
    else if (tEntryX > tEntryY) {
        normal.x = (velocity.x < 0) ? 1.f : -1.f;
    }
    else {
        normal.y = (velocity.y < 0) ? 1.f : -1.f;
    }

    // Нормализация
    float len = std::hypot(normal.x, normal.y);
    if (len > 0) normal /= len;

    return { normal, tEntry };
}

void BlockCollision::checkBlockCollisions(
    Ball& ball,
    std::vector<std::unique_ptr<BaseBlock>>& blocks,
    int& score,
    float dt,
    sf::Sound& blockSound,
    sf::Sound& rockSound,
    SpatialGrid& spatialGrid
) {
    const sf::Vector2f startPos = ball.getPosition();
    const sf::Vector2f vel = ball.getVelocity();
    const float radius = ball.getBounds().width * 0.5f;

    // Broad-phase: границы движения мяча
    sf::FloatRect broadPhaseBounds(
        std::min(startPos.x, startPos.x + vel.x * dt) - radius,
        std::min(startPos.y, startPos.y + vel.y * dt) - radius,
        std::abs(vel.x * dt) + 2 * radius,
        std::abs(vel.y * dt) + 2 * radius
    );

    // Получаем блоки через сетку
    auto potentialBlocks = spatialGrid.getPotentialCollisions(broadPhaseBounds);
    std::unordered_set<BaseBlock*> uniqueBlocks(potentialBlocks.begin(), potentialBlocks.end());

    // Сбор и сортировка всех столкновений
    std::vector<std::pair<BaseBlock*, std::pair<sf::Vector2f, float>>> collisions;
    for (auto* blk : uniqueBlocks) {
        if (blk->isDestroyed()) continue;

        auto collision = sweepAABB(startPos, vel, dt, blk->getBounds(), radius);
        if (collision.first != sf::Vector2f{ 0.f, 0.f }) {
            collisions.emplace_back(blk, collision);
        }
    }

    // Сортировка по времени столкновения (tEntry)
    std::sort(collisions.begin(), collisions.end(),
        [](const auto& a, const auto& b) {
            return a.second.second < b.second.second;
        });

    // Обработка только первого столкновения
    bool collisionHandled = false;
    for (auto& [blk, collision] : collisions) {
        if (blk->isDestroyed() || collisionHandled) continue;

        const auto& [normal, tEntry] = collision;

        // Коррекция позиции
        const sf::Vector2f correction = vel * tEntry * dt;
        ball.getSprite().setPosition(startPos + correction);

        // Отражение
        ball.reflect(normal);
        ball.getSprite().move(normal * (radius + 1.f));

        // Увеличение скорости мяча (восстановлено!)
        if (!blk->IsRock()) {
            ball.increaseSpeedFactor(SPEED_GROWTH);
        }

        // Звук и уничтожение блока
        blk->IsRock() ? rockSound.play() : blockSound.play();
        if (blk->hit()) score += blk->getPoints();

        collisionHandled = true;
        break;
    }

    // Обновление сетки после изменений
    if (collisionHandled) {
        spatialGrid.clear();
        for (auto& block : blocks) {
            if (!block->isDestroyed()) spatialGrid.addBlock(block.get());
        }
    }

    // Удаление уничтоженных блоков
    blocks.erase(
        std::remove_if(blocks.begin(), blocks.end(),
            [](const auto& blk) { return blk->isDestroyed(); }),
        blocks.end()
    );
}