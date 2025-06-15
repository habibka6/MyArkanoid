#pragma once
#include "BaseBlock.h"
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>

namespace Arkanoid {

    struct CellHash {
        std::size_t operator()(const std::pair<int, int>& cell) const {
            return std::hash<int>()(cell.first) ^
                (std::hash<int>()(cell.second) << 1);
        }
    };

    class SpatialGrid {
    public:
        SpatialGrid(float width, float height, float cellSize);
        ~SpatialGrid() = default;

        void clear();
        void addBlock(BaseBlock* block);

        void update(const std::vector<std::unique_ptr<BaseBlock>>& blocks);

        std::vector<BaseBlock*> getPotentialCollisions(const sf::FloatRect& bounds);

    private:
        float cellSize;
        int cols, rows;

        std::unordered_map<std::pair<int, int>, std::vector<BaseBlock*>, CellHash> grid;

        std::vector<std::pair<int, int>> getCellsForBounds(const sf::FloatRect& bounds);
    };

} 
