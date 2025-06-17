#include "LevelManager.h"
#include "Config.h"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace Arkanoid {

    LevelManager::LevelManager()
        : maxLevels(13),
        blockWidth(Config::Block::WIDTH),
        blockHeight(Config::Block::HEIGHT),
        defaultSpacing(5.0f) {

        loadLevelsFromJSON();

        if (predefinedLevels.empty()) {
            predefinedLevels.push_back(createDefaultLevel());
        }

        loadLevel(1);
    }

    // Загрузка уровня по номеру
    bool LevelManager::loadLevel(int levelNumber) {
        if (levelNumber <= 0) return false;

        for (const auto& level : predefinedLevels) {
            if (level.levelNumber == levelNumber) {
                currentLevel = level;
                return true;
            }
        }

        std::string filename = "assets/levels/level_" + std::to_string(levelNumber) + ".json";
        if (loadSingleLevel(filename, levelNumber)) {
            return true;
        }

        currentLevel = createDefaultLevel();
        currentLevel.levelNumber = levelNumber;
        currentLevel.name = "Level " + std::to_string(levelNumber);
        return true;
    }

    // Загрузка всех уровней из JSON
    void LevelManager::loadLevelsFromJSON() {
        predefinedLevels.clear();

        std::ifstream file("assets/levels/levels.json");
        if (file.is_open()) {
            try {
                nlohmann::json json;
                file >> json;

                if (json.contains("levels") && json["levels"].is_array()) {
                    for (size_t i = 0; i < json["levels"].size(); ++i) {
                        try {
                            LevelData level = parseJSONLevel(json["levels"][i], static_cast<int>(i + 1));
                            if (validateLevelData(level)) {
                                predefinedLevels.push_back(level);
                            }
                        }
                        catch (const std::exception& e) {
                            std::cerr << "Error parsing level " << (i + 1) << ": " << e.what() << std::endl;
                        }
                    }
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error loading levels.json: " << e.what() << std::endl;
            }
        }

        // Загрузка отдельных файлов уровней
        for (int i = 1; i <= maxLevels; ++i) {
            std::string filename = "assets/levels/level_" + std::to_string(i) + ".json";
            loadSingleLevel(filename, i);
        }
    }

    // Загрузка одного уровня из файла
    bool LevelManager::loadSingleLevel(const std::string& filepath, int levelNumber) {
        std::ifstream file(filepath);
        if (!file.is_open()) return false;

        try {
            nlohmann::json json;
            file >> json;

            LevelData level = parseJSONLevel(json, levelNumber);
            if (validateLevelData(level)) {
                auto it = std::find_if(predefinedLevels.begin(), predefinedLevels.end(),
                    [levelNumber](const LevelData& l) { return l.levelNumber == levelNumber; });

                if (it != predefinedLevels.end()) {
                    *it = level;
                }
                else {
                    predefinedLevels.push_back(level);
                }
                return true;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error loading " << filepath << ": " << e.what() << std::endl;
        }

        return false;
    }

    // Парсинг уровня из JSON
    LevelData LevelManager::parseJSONLevel(const nlohmann::json& json, int levelNumber) {
        LevelData level;

        level.levelNumber = json.value("number", levelNumber);
        level.name = json.value("name", "Level " + std::to_string(levelNumber));
        level.description = json.value("description", "");

        level.centerHorizontally = json.value("centerHorizontally", true);
        level.centerVertically = json.value("centerVertically", false);
        level.marginTop = json.value("marginTop", 100.0f);
        level.marginSides = json.value("marginSides", 50.0f);
        level.blockSpacing = json.value("spacing", defaultSpacing);

        // Чтение макета уровня
        if (json.contains("layout") && json["layout"].is_array()) {
            for (const auto& row : json["layout"]) {
                std::vector<int> levelRow;
                if (row.is_array()) {
                    for (const auto& cell : row) {
                        levelRow.push_back(cell.is_number() ? cell.get<int>() : 0);
                    }
                }
                level.layout.push_back(levelRow);
            }
        }

        level.rows = static_cast<int>(level.layout.size());
        level.cols = level.rows > 0 ? static_cast<int>(level.layout[0].size()) : 0;

        level.startPosition = calculateCenteredStartPosition(level);

        return level;
    }

    // Центрирование макета уровня
    sf::Vector2f LevelManager::calculateCenteredStartPosition(const LevelData& level) const {
        float windowWidth = static_cast<float>(Config::Window::WIDTH);
        float windowHeight = static_cast<float>(Config::Window::HEIGHT);

        float totalWidth = level.cols * blockWidth + (level.cols - 1) * level.blockSpacing;
        float totalHeight = level.rows * blockHeight + (level.rows - 1) * level.blockSpacing;

        float x, y;

        if (level.centerHorizontally) {
            x = (windowWidth - totalWidth) / 2.0f;
        }
        else {
            x = level.marginSides;
        }

        if (level.centerVertically) {
            y = (windowHeight - totalHeight) / 2.0f;
        }
        else {
            y = level.marginTop;
        }

        return sf::Vector2f(x, y);
    }

    // Генерация блоков для текущего уровня
    std::vector<std::unique_ptr<BaseBlock>> LevelManager::generateBlocks() const {
        std::vector<std::unique_ptr<BaseBlock>> blocks;

        for (int row = 0; row < currentLevel.rows; ++row) {
            for (int col = 0; col < currentLevel.cols; ++col) {
                if (row < static_cast<int>(currentLevel.layout.size()) &&
                    col < static_cast<int>(currentLevel.layout[row].size())) {

                    int blockType = currentLevel.layout[row][col];
                    if (blockType != 0) {
                        sf::Vector2f pos = calculateBlockPosition(row, col, currentLevel);
                        auto block = createBlock(blockType, pos.x, pos.y);
                        if (block) {
                            blocks.push_back(std::move(block));
                        }
                    }
                }
            }
        }

        return blocks;
    }

    // Расчёт позиции блока
    sf::Vector2f LevelManager::calculateBlockPosition(int row, int col, const LevelData& level) const {
        float x = level.startPosition.x + col * (blockWidth + level.blockSpacing);
        float y = level.startPosition.y + row * (blockHeight + level.blockSpacing);
        return sf::Vector2f(x, y);
    }

    // Создание блока по типу
    std::unique_ptr<BaseBlock> LevelManager::createBlock(int blockType, float x, float y) const {
        switch (blockType) {
        case 1: // Зелёный блок
            return std::make_unique<Block>(x, y, Block::Type::Green);
        case 2: // Жёлтый блок
            return std::make_unique<Block>(x, y, Block::Type::Yellow);
        case 3: // Красный блок
            return std::make_unique<Block>(x, y, Block::Type::Red);
        case 9: // Камень
            return std::make_unique<Rock>(x, y);
        default:
            return nullptr;
        }
    }

    bool LevelManager::hasNextLevel() const {
        return currentLevel.levelNumber < maxLevels;
    }

    bool LevelManager::nextLevel() {
        if (hasNextLevel()) {
            return loadLevel(currentLevel.levelNumber + 1);
        }
        return false;
    }

    // Сброс к первому уровню
    void LevelManager::resetToFirstLevel() {
        loadLevel(1);
    }

    // Проверка завершения уровня (все разрушаемые блоки уничтожены)
    bool LevelManager::isLevelComplete(const std::vector<std::unique_ptr<BaseBlock>>& blocks) const {
        for (const auto& block : blocks) {
            if (!block || !block->isActive()) continue;
            if (block->getBlockType() != BlockType::Rock) {
                return false;
            }
        }
        return true;
    }

    // Валидация данных уровня
    bool LevelManager::validateLevelData(const LevelData& level) const {
        if (level.rows <= 0 || level.cols <= 0) return false;
        if (level.layout.size() != static_cast<size_t>(level.rows)) return false;

        for (const auto& row : level.layout) {
            if (row.size() != static_cast<size_t>(level.cols)) return false;
        }

        return true;
    }

    // Создание дефолтного уровня
    LevelData LevelManager::createDefaultLevel() const {
        LevelData level;
        level.levelNumber = 1;
        level.name = "Default Level";
        level.description = "A simple default level";
        level.rows = 3;
        level.cols = 11;
        level.centerHorizontally = true;
        level.centerVertically = false;
        level.marginTop = 100.0f;
        level.marginSides = 50.0f;
        level.blockSpacing = defaultSpacing;
        level.startPosition = calculateCenteredStartPosition(level);

        level.layout = {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
        };

        return level;
    }
    void LevelManager::setBlockSize(float width, float height) {
        blockWidth = width;
        blockHeight = height;
    }

    void LevelManager::setDefaultSpacing(float spacing) {
        defaultSpacing = spacing;
    }

} // namespace Arkanoid
