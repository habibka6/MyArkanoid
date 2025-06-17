#pragma once
#include "BaseBlock.h"
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>

namespace Arkanoid {

    // Хэш-функция для пары целых чисел (координаты ячейки сетки)
    struct CellHash {
        std::size_t operator()(const std::pair<int, int>& cell) const {
            return std::hash<int>()(cell.first) ^
                (std::hash<int>()(cell.second) << 1);
        }
    };
    // Пространственная сетка для оптимизации обнаружения коллизий мяча с блоками
    class SpatialGrid {
    public:
        SpatialGrid(float width, float height, float cellSize);
        ~SpatialGrid() = default;

        void clear();

        // Добавляет блок в соответствующие ячейки сетки
        void addBlock(BaseBlock* block);

        // Обновляет сетку на основе текущего списка блоков
        void update(const std::vector<std::unique_ptr<BaseBlock>>& blocks);

        // Возвращает все блоки, которые потенциально могут пересекаться с заданными границами
        std::vector<BaseBlock*> getPotentialCollisions(const sf::FloatRect& bounds);

    private:
        float cellSize; // Размер одной ячейки сетки
        int cols, rows;

        std::unordered_map<std::pair<int, int>, std::vector<BaseBlock*>, CellHash> grid;

        std::vector<std::pair<int, int>> getCellsForBounds(const sf::FloatRect& bounds);
    };

} 
