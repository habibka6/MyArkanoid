#include "SpatialGrid.h"

SpatialGrid::SpatialGrid(float width, float height, float cellSize)
    : cellSize(cellSize) {
    cols = static_cast<int>(width / cellSize) + 1;
    rows = static_cast<int>(height / cellSize) + 1;
    cells.resize(cols * rows);
}

void SpatialGrid::update(const std::vector<std::unique_ptr<BaseBlock>>& blocks) {
    clear();
    for (auto& block : blocks) {
        if (!block->isDestroyed()) {
            addBlock(block.get());
        }
    }
}

void SpatialGrid::clear() {
    for (auto& cell : cells) cell.clear();
}

void SpatialGrid::addBlock(BaseBlock* block) {
    sf::FloatRect bounds = block->getBounds();
    int left = static_cast<int>(bounds.left / cellSize);
    int right = static_cast<int>((bounds.left + bounds.width) / cellSize);
    int top = static_cast<int>(bounds.top / cellSize);
    int bottom = static_cast<int>((bounds.top + bounds.height) / cellSize);

    for (int x = left; x <= right; ++x) {
        for (int y = top; y <= bottom; ++y) {
            int index = y * cols + x;
            if (index >= 0 && index < cells.size()) {
                cells[index].push_back(block);
            }
        }
    }
}

std::vector<BaseBlock*> SpatialGrid::getPotentialCollisions(const sf::FloatRect& bounds) {
    std::vector<BaseBlock*> result;
    int left = static_cast<int>(bounds.left / cellSize);
    int right = static_cast<int>((bounds.left + bounds.width) / cellSize);
    int top = static_cast<int>(bounds.top / cellSize);
    int bottom = static_cast<int>((bounds.top + bounds.height) / cellSize);

    for (int x = left; x <= right; ++x) {
        for (int y = top; y <= bottom; ++y) {
            int index = y * cols + x;
            if (index >= 0 && index < cells.size()) {
                for (auto block : cells[index]) {
                    result.push_back(block);
                }
            }
        }
    }
    return result;
}