#include "SpatialGrid.h"
#include <cmath>

namespace Arkanoid {

    SpatialGrid::SpatialGrid(float width, float height, float cellSize)
        : cellSize(cellSize) {
        cols = static_cast<int>(std::ceil(width / cellSize));
        rows = static_cast<int>(std::ceil(height / cellSize));
    }

    void SpatialGrid::clear() {
        grid.clear();
    }

    void SpatialGrid::addBlock(BaseBlock* block) {
        if (!block || !block->isActive()) return;

        std::vector<std::pair<int, int>> cells = getCellsForBounds(block->getBounds());
        for (const auto& cell : cells) {
            grid[cell].push_back(block);
        }
    }

    void SpatialGrid::update(const std::vector<std::unique_ptr<BaseBlock>>& blocks) {
        clear();
        for (const auto& block : blocks) {
            if (block && block->isActive()) {
                addBlock(block.get());
            }
        }
    }

    std::vector<BaseBlock*> SpatialGrid::getPotentialCollisions(const sf::FloatRect& bounds) {
        std::vector<BaseBlock*> result;
        std::vector<std::pair<int, int>> cells = getCellsForBounds(bounds);

        for (const auto& cell : cells) {
            auto it = grid.find(cell);
            if (it != grid.end()) {
                for (BaseBlock* block : it->second) {
                    if (block && block->isActive()) {
                        result.push_back(block);
                    }
                }
            }
        }

        return result;
    }

    std::vector<std::pair<int, int>> SpatialGrid::getCellsForBounds(const sf::FloatRect& bounds) {
        std::vector<std::pair<int, int>> cells;

        int minX = std::max(0, static_cast<int>(bounds.left / cellSize));
        int maxX = std::min(cols - 1, static_cast<int>((bounds.left + bounds.width) / cellSize));
        int minY = std::max(0, static_cast<int>(bounds.top / cellSize));
        int maxY = std::min(rows - 1, static_cast<int>((bounds.top + bounds.height) / cellSize));

        for (int x = minX; x <= maxX; ++x) {
            for (int y = minY; y <= maxY; ++y) {
                cells.push_back({ x, y });
            }
        }

        return cells;
    }

} 
