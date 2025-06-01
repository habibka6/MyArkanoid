#pragma once
#include "BaseBlock.h"
#include "Block.h"
#include "Rock.h"
#include <vector>
#include <memory>
#include <string>

namespace Arkanoid {

    struct LevelData {
        int levelNumber;
        std::string name;
        int rows;
        int cols;
        std::vector<std::vector<int>> layout; // 0=empty, 1=green, 2=yellow, 3=red, 9=rock
        float blockSpacing;
        sf::Vector2f startPosition;
    };

    class LevelManager {
    public:
        LevelManager();
        ~LevelManager() = default;

        // Управление уровнями
        bool loadLevel(int levelNumber);
        bool loadLevelFromFile(const std::string& filename);
        std::vector<std::unique_ptr<BaseBlock>> generateBlocks() const;

        // Информация о текущем уровне
        const LevelData& getCurrentLevel() const;
        int getCurrentLevelNumber() const;
        bool hasNextLevel() const;
        bool hasPreviousLevel() const;

        // Навигация по уровням
        bool nextLevel();
        bool previousLevel();
        void resetToFirstLevel();

        // Генерация уровней
        LevelData generateRandomLevel(int levelNumber, int difficulty) const;

        // Проверка завершения уровня
        bool isLevelComplete(const std::vector<std::unique_ptr<BaseBlock>>& blocks) const;

        // Настройки генерации
        void setBlockSize(float width, float height);
        void setStartPosition(float x, float y);
        void setSpacing(float spacing);

    private:
        LevelData currentLevel;
        int maxLevels;

        // Настройки размещения блоков
        float blockWidth;
        float blockHeight;
        float blockSpacing;
        sf::Vector2f levelStartPosition;

        // Предопределенные уровни
        void initializePredefinedLevels();
        std::vector<LevelData> predefinedLevels;

        // Вспомогательные методы
        std::unique_ptr<BaseBlock> createBlock(int blockType, float x, float y) const;
        sf::Vector2f calculateBlockPosition(int row, int col) const;
        LevelData createDefaultLevel() const;
        bool validateLevelData(const LevelData& level) const;

        // Генерация паттернов
        std::vector<std::vector<int>> generatePattern(int rows, int cols, int difficulty) const;
        std::vector<std::vector<int>> generateWavePattern(int rows, int cols) const;
        std::vector<std::vector<int>> generateCheckerPattern(int rows, int cols) const;
        std::vector<std::vector<int>> generatePyramidPattern(int rows, int cols) const;
    };

} // namespace Arkanoid
