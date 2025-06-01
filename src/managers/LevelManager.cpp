#include "LevelManager.h"
#include "Config.h"
#include <random>
#include <iostream>
#include <fstream>

namespace Arkanoid {

    LevelManager::LevelManager()
        : maxLevels(10),
        blockWidth(Config::Block::WIDTH),
        blockHeight(Config::Block::HEIGHT),
        blockSpacing(5.0f),
        levelStartPosition(50.0f, 100.0f) {

        initializePredefinedLevels();
        loadLevel(1); // Загружаем первый уровень
    }

    bool LevelManager::loadLevel(int levelNumber) {
        if (levelNumber <= 0) return false;

        // Если уровень есть в предопределенных, используем его
        if (levelNumber <= static_cast<int>(predefinedLevels.size())) {
            currentLevel = predefinedLevels[levelNumber - 1];
            return true;
        }

        // Иначе генерируем случайный уровень
        int difficulty = std::min(5, (levelNumber - 1) / 2 + 1); // Увеличиваем сложность каждые 2 уровня
        currentLevel = generateRandomLevel(levelNumber, difficulty);
        return true;
    }

    bool LevelManager::loadLevelFromFile(const std::string& filename) {
        // Заглушка для загрузки из файла
        // В будущем здесь будет парсинг JSON/XML
        return false;
    }

    std::vector<std::unique_ptr<BaseBlock>> LevelManager::generateBlocks() const {
        std::vector<std::unique_ptr<BaseBlock>> blocks;

        for (int row = 0; row < currentLevel.rows; ++row) {
            for (int col = 0; col < currentLevel.cols; ++col) {
                int blockType = currentLevel.layout[row][col];
                if (blockType != 0) { // 0 = пустое место
                    sf::Vector2f pos = calculateBlockPosition(row, col);
                    auto block = createBlock(blockType, pos.x, pos.y);
                    if (block) {
                        blocks.push_back(std::move(block));
                    }
                }
            }
        }

        return blocks;
    }

    const LevelData& LevelManager::getCurrentLevel() const {
        return currentLevel;
    }

    int LevelManager::getCurrentLevelNumber() const {
        return currentLevel.levelNumber;
    }

    bool LevelManager::hasNextLevel() const {
        return currentLevel.levelNumber < maxLevels;
    }

    bool LevelManager::hasPreviousLevel() const {
        return currentLevel.levelNumber > 1;
    }

    bool LevelManager::nextLevel() {
        if (hasNextLevel()) {
            return loadLevel(currentLevel.levelNumber + 1);
        }
        return false;
    }

    bool LevelManager::previousLevel() {
        if (hasPreviousLevel()) {
            return loadLevel(currentLevel.levelNumber - 1);
        }
        return false;
    }

    void LevelManager::resetToFirstLevel() {
        loadLevel(1);
    }

    LevelData LevelManager::generateRandomLevel(int levelNumber, int difficulty) const {
        LevelData level;
        level.levelNumber = levelNumber;
        level.name = "Generated Level " + std::to_string(levelNumber);
        level.rows = 5 + difficulty;
        level.cols = 10;
        level.blockSpacing = blockSpacing;
        level.startPosition = levelStartPosition;

        level.layout = generatePattern(level.rows, level.cols, difficulty);

        return level;
    }

    bool LevelManager::isLevelComplete(const std::vector<std::unique_ptr<BaseBlock>>& blocks) const {
        for (const auto& block : blocks) {
            if (block->isActive() && block->getBlockType() != BlockType::Rock) {
                return false; // Есть еще разрушаемые блоки
            }
        }
        return true;
    }

    void LevelManager::setBlockSize(float width, float height) {
        blockWidth = width;
        blockHeight = height;
    }

    void LevelManager::setStartPosition(float x, float y) {
        levelStartPosition = { x, y };
    }

    void LevelManager::setSpacing(float spacing) {
        blockSpacing = spacing;
    }

    void LevelManager::initializePredefinedLevels() {
        // Уровень 1 - простой
        LevelData level1;
        level1.levelNumber = 1;
        level1.name = "Getting Started";
        level1.rows = 3;
        level1.cols = 8;
        level1.blockSpacing = blockSpacing;
        level1.startPosition = levelStartPosition;
        level1.layout = {
            {1, 1, 1, 1, 1, 1, 1, 1},
            {2, 2, 2, 2, 2, 2, 2, 2},
            {1, 1, 1, 1, 1, 1, 1, 1}
        };
        predefinedLevels.push_back(level1);

        // Уровень 2 - с камнями
        LevelData level2;
        level2.levelNumber = 2;
        level2.name = "Rocky Road";
        level2.rows = 4;
        level2.cols = 8;
        level2.blockSpacing = blockSpacing;
        level2.startPosition = levelStartPosition;
        level2.layout = {
            {1, 1, 1, 1, 1, 1, 1, 1},
            {2, 9, 2, 2, 2, 2, 9, 2},
            {3, 3, 3, 3, 3, 3, 3, 3},
            {1, 1, 1, 1, 1, 1, 1, 1}
        };
        predefinedLevels.push_back(level2);

        // Уровень 3 - сложный паттерн
        LevelData level3;
        level3.levelNumber = 3;
        level3.name = "Pyramid Power";
        level3.rows = 5;
        level3.cols = 9;
        level3.blockSpacing = blockSpacing;
        level3.startPosition = levelStartPosition;
        level3.layout = generatePyramidPattern(5, 9);
        predefinedLevels.push_back(level3);
    }

    std::unique_ptr<BaseBlock> LevelManager::createBlock(int blockType, float x, float y) const {
        switch (blockType) {
        case 1: // Зеленый
            return std::make_unique<Block>(x, y, Block::Type::Green);
        case 2: // Желтый
            return std::make_unique<Block>(x, y, Block::Type::Yellow);
        case 3: // Красный
            return std::make_unique<Block>(x, y, Block::Type::Red);
        case 9: // Камень
            return std::make_unique<Rock>(x, y);
        default:
            return nullptr;
        }
    }

    sf::Vector2f LevelManager::calculateBlockPosition(int row, int col) const {
        float x = levelStartPosition.x + col * (blockWidth + blockSpacing);
        float y = levelStartPosition.y + row * (blockHeight + blockSpacing);
        return { x, y };
    }

    LevelData LevelManager::createDefaultLevel() const {
        LevelData level;
        level.levelNumber = 1;
        level.name = "Default";
        level.rows = 3;
        level.cols = 8;
        level.blockSpacing = blockSpacing;
        level.startPosition = levelStartPosition;
        level.layout = {
            {1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 1}
        };
        return level;
    }

    bool LevelManager::validateLevelData(const LevelData& level) const {
        if (level.rows <= 0 || level.cols <= 0) return false;
        if (level.layout.size() != static_cast<size_t>(level.rows)) return false;

        for (const auto& row : level.layout) {
            if (row.size() != static_cast<size_t>(level.cols)) return false;
        }

        return true;
    }

    std::vector<std::vector<int>> LevelManager::generatePattern(int rows, int cols, int difficulty) const {
        std::random_device rd;
        std::mt19937 gen(rd());

        // Выбираем тип паттерна в зависимости от сложности
        std::uniform_int_distribution<int> patternDist(0, 2);
        int patternType = patternDist(gen);

        switch (patternType) {
        case 0:
            return generateWavePattern(rows, cols);
        case 1:
            return generateCheckerPattern(rows, cols);
        case 2:
        default:
            return generatePyramidPattern(rows, cols);
        }
    }

    std::vector<std::vector<int>> LevelManager::generateWavePattern(int rows, int cols) const {
        std::vector<std::vector<int>> pattern(rows, std::vector<int>(cols, 0));

        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                // Создаем волнообразный паттерн
                if ((row + col) % 3 != 0) {
                    int blockType = 1 + (row % 3); // 1, 2 или 3
                    pattern[row][col] = blockType;
                }
            }
        }

        return pattern;
    }

    std::vector<std::vector<int>> LevelManager::generateCheckerPattern(int rows, int cols) const {
        std::vector<std::vector<int>> pattern(rows, std::vector<int>(cols, 0));

        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                // Шахматный паттерн с камнями
                if ((row + col) % 2 == 0) {
                    if (row == 1 && col % 4 == 0) {
                        pattern[row][col] = 9; // Камень
                    }
                    else {
                        pattern[row][col] = 1 + (row % 3);
                    }
                }
            }
        }

        return pattern;
    }

    std::vector<std::vector<int>> LevelManager::generatePyramidPattern(int rows, int cols) const {
        std::vector<std::vector<int>> pattern(rows, std::vector<int>(cols, 0));

        int center = cols / 2;
        for (int row = 0; row < rows; ++row) {
            int width = std::min(cols, 2 * row + 1);
            int start = center - width / 2;
            int end = start + width;

            for (int col = start; col < end && col < cols; ++col) {
                if (col >= 0) {
                    pattern[row][col] = 1 + (row % 3); // Градиент цветов сверху вниз
                }
            }
        }

        return pattern;
    }

} // namespace Arkanoid
