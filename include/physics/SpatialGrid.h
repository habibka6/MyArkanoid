#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "BaseBlock.h"

class SpatialGrid {
public:
    SpatialGrid(float width, float height, float cellSize);
    void clear();
    void addBlock(BaseBlock* block);
    std::vector<BaseBlock*> getPotentialCollisions(const sf::FloatRect& bounds);

private:
    float cellSize;
    int cols, rows;
    std::vector<std::vector<BaseBlock*>> cells;

    int getCellIndex(float x, float y) const;
};