#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "BaseBlock.h"

class SpatialGrid {
public:
    SpatialGrid(float width, float height, float cellSize);
    void clear();
    void addBlock(BaseBlock* block);
    void update(const std::vector<std::unique_ptr<BaseBlock>>& blocks); 
    std::vector<BaseBlock*> getPotentialCollisions(const sf::FloatRect& bounds);

private:
    float cellSize;
    int cols, rows;
    std::vector<std::vector<BaseBlock*>> cells;
};