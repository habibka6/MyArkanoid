#pragma once
#include "BaseBlock.h"
#include "Block.h"
#include "Rock.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

namespace Arkanoid {

    // параметры уровня
    struct LevelData {
        int levelNumber;
        std::string name;
        std::string description;
        int rows;
        int cols;
        std::vector<std::vector<int>> layout;
        sf::Vector2f startPosition;
        float blockSpacing;

        bool centerHorizontally;
        bool centerVertically;
        float marginTop;
        float marginSides;
    };

    class LevelManager {
    private:
        std::vector<LevelData> predefinedLevels;
        LevelData currentLevel;

        float blockWidth;
        float blockHeight;
        float defaultSpacing;
        int maxLevels;

    public:
        LevelManager();
        ~LevelManager() = default;

        // Основные методы
        bool loadLevel(int levelNumber);
        std::vector<std::unique_ptr<BaseBlock>> generateBlocks() const;

        // Геттеры
        const LevelData& getCurrentLevel() const { return currentLevel; }
        int getCurrentLevelNumber() const { return currentLevel.levelNumber; }
        int getMaxLevels() const { return maxLevels; }

        // Навигация
        bool hasNextLevel() const;
        bool nextLevel();
        void resetToFirstLevel();

        // Проверка завершения
        bool isLevelComplete(const std::vector<std::unique_ptr<BaseBlock>>& blocks) const;

        // Настройки
        void setBlockSize(float width, float height);
        void setDefaultSpacing(float spacing);

    private:
        // JSON методы
        void loadLevelsFromJSON();
        bool loadSingleLevel(const std::string& filepath, int levelNumber);
        LevelData parseJSONLevel(const nlohmann::json& json, int levelNumber);

        // Утилиты создания блоков
        std::unique_ptr<BaseBlock> createBlock(int blockType, float x, float y) const;
        sf::Vector2f calculateBlockPosition(int row, int col, const LevelData& level) const;
        sf::Vector2f calculateCenteredStartPosition(const LevelData& level) const;

        // Валидация
        bool validateLevelData(const LevelData& level) const;
        LevelData createDefaultLevel() const;
    };

} // namespace Arkanoid
