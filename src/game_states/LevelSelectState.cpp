#include "LevelSelectState.h"
#include "MainMenuState.h"
#include "GameState.h"
#include "GameEngine.h"
#include "AssetManager.h"
#include "SoundManager.h"
#include "Config.h"
#include <algorithm>
#include <sstream>

namespace Arkanoid {

    LevelSelectState::LevelSelectState(GameEngine& engine)
        : State(engine), selectedLevelIndex(0), currentPage(0) {

        totalPages = (MAX_LEVELS + LEVELS_PER_PAGE - 1) / LEVELS_PER_PAGE;
    }

    void LevelSelectState::enter() {
        loadAssets();
        initializeUI();
        setupCurrentPage();
        updatePageDisplay();
        initializeInputBindings();

        try {
            SoundManager::getInstance().playMusic("menu_music.ogg", true);
        }
        catch (...) { }
    }

    void LevelSelectState::exit(){}

    void LevelSelectState::update(float deltaTime) {
        inputSystem.update();
        updateSelection();
    }

    void LevelSelectState::render(sf::RenderWindow& window) {
     
        if (backgroundSprite.getTexture()) {
            window.draw(backgroundSprite);
        }

        
        window.draw(titleText);
        window.draw(pageText);
        window.draw(instructionText);

        
        for (const auto& button : levelButtons) {
            window.draw(button.background);
            window.draw(button.border);
            window.draw(button.numberText);
        }
    }

    void LevelSelectState::initializeInputBindings() {
  
        inputSystem.bindKeyPress(sf::Keyboard::Up,
            std::make_unique<LambdaCommand>([this]() {
                navigateUp();
                }));

        inputSystem.bindKeyPress(sf::Keyboard::Down,
            std::make_unique<LambdaCommand>([this]() {
                navigateDown();
                }));

        inputSystem.bindKeyPress(sf::Keyboard::Left,
            std::make_unique<LambdaCommand>([this]() {
                navigateLeft();
                }));

        inputSystem.bindKeyPress(sf::Keyboard::Right,
            std::make_unique<LambdaCommand>([this]() {
                navigateRight();
                }));


        inputSystem.bindKeyPress(sf::Keyboard::Enter,
            std::make_unique<LambdaCommand>([this]() {
                selectLevel();
                }));

        inputSystem.bindKeyPress(sf::Keyboard::Space,
            std::make_unique<LambdaCommand>([this]() {
                selectLevel();
                }));

        inputSystem.bindKeyPress(sf::Keyboard::Q,
            std::make_unique<LambdaCommand>([this]() {
                previousPage();
                }));

        inputSystem.bindKeyPress(sf::Keyboard::PageUp,
            std::make_unique<LambdaCommand>([this]() {
                previousPage();
                }));

        inputSystem.bindKeyPress(sf::Keyboard::E,
            std::make_unique<LambdaCommand>([this]() {
                nextPage();
                }));

        inputSystem.bindKeyPress(sf::Keyboard::PageDown,
            std::make_unique<LambdaCommand>([this]() {
                nextPage();
                }));

        // Возврат в меню
        inputSystem.bindKeyPress(sf::Keyboard::Escape,
            std::make_unique<LambdaCommand>([this]() {
                returnToMenu();
                }));
    }

    void LevelSelectState::handleEvent(const sf::Event& event) {
        inputSystem.processEvent(event);
        // Мышь
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y));

                for (size_t i = 0; i < levelButtons.size(); ++i) {
                    if (levelButtons[i].bounds.contains(mousePos)) {
                        selectedLevelIndex = static_cast<int>(i);
                        selectLevel();
                        break;
                    }
                }
            }
        }

        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x),
                static_cast<float>(event.mouseMove.y));

            for (size_t i = 0; i < levelButtons.size(); ++i) {
                if (levelButtons[i].bounds.contains(mousePos)) {
                    selectedLevelIndex = static_cast<int>(i);
                    break;
                }
            }
        }
    }

    void LevelSelectState::loadAssets() {
        try {

            try {
                font = AssetManager::getInstance().getFont("retro.ttf");
            }
            catch (...) {

            }

            try {
                sf::Texture& bgTexture = AssetManager::getInstance().getTexture("menu_background.jpg");
                backgroundSprite.setTexture(bgTexture);

                sf::Vector2u windowSize = engine.getWindow().getSize();
                sf::Vector2u textureSize = bgTexture.getSize();

                if (textureSize.x > 0 && textureSize.y > 0) {
                    backgroundSprite.setScale(
                        static_cast<float>(windowSize.x) / textureSize.x,
                        static_cast<float>(windowSize.y) / textureSize.y
                    );
                }
            }
            catch (...) {}
        }
        catch (...) { }
    }

    void LevelSelectState::initializeUI() {
        float windowWidth = static_cast<float>(Config::Window::WIDTH);
        float windowHeight = static_cast<float>(Config::Window::HEIGHT);

        // Заголовок
        setupText(titleText, "SELECT LEVEL", 0, 50, 48);
        centerTextHorizontally(titleText, 50);
        titleText.setFillColor(sf::Color::Cyan);
        titleText.setOutlineColor(sf::Color::Blue);
        titleText.setOutlineThickness(2);

        // Инструкции
        setupText(instructionText,
            "ARROWS - Navigate    ENTER - Select    Q/E - Pages    ESC - Back",
            0, windowHeight - 40, 18);
        centerTextHorizontally(instructionText, windowHeight - 40);
        instructionText.setFillColor(sf::Color::Yellow);
    }

    void LevelSelectState::setupCurrentPage() {
        levelButtons.clear();

        int startLevel = currentPage * LEVELS_PER_PAGE + 1;
        int endLevel = std::min(startLevel + LEVELS_PER_PAGE - 1, MAX_LEVELS);

        // Размеры кнопки
        float buttonWidth = 120.0f;
        float buttonHeight = 80.0f;
        float spacingX = 140.0f;
        float spacingY = 100.0f;

        // Центрирование сетки
        float totalGridWidth = LEVELS_PER_ROW * buttonWidth + (LEVELS_PER_ROW - 1) * (spacingX - buttonWidth);
        float totalGridHeight = LEVELS_PER_COLUMN * buttonHeight + (LEVELS_PER_COLUMN - 1) * (spacingY - buttonHeight);

        float windowWidth = static_cast<float>(Config::Window::WIDTH);
        float windowHeight = static_cast<float>(Config::Window::HEIGHT);

        float startX = (windowWidth - totalGridWidth) / 2.0f;
        float startY = (windowHeight - totalGridHeight) / 2.0f + 20.0f; 

        for (int level = startLevel; level <= endLevel; ++level) {
            LevelButton button(level);

            int localIndex = level - startLevel;
            int row = localIndex / LEVELS_PER_ROW;
            int col = localIndex % LEVELS_PER_ROW;

            float x = startX + col * spacingX;
            float y = startY + row * spacingY;

            // Настройка фона и границы
            button.background.setSize(sf::Vector2f(buttonWidth, buttonHeight));
            button.background.setPosition(x, y);

            button.border.setSize(sf::Vector2f(buttonWidth + 4, buttonHeight + 4));
            button.border.setPosition(x - 2, y - 2);
            button.border.setFillColor(sf::Color::Transparent);
            button.border.setOutlineThickness(2);

            // Настройка текста
            if (!font.getInfo().family.empty()) {
                button.numberText.setFont(font);
            }
            button.numberText.setString(std::to_string(level));
            button.numberText.setCharacterSize(30);
            button.numberText.setFillColor(sf::Color::White);

            // Центрируем текст в кнопке
            sf::FloatRect textBounds = button.numberText.getLocalBounds();
            button.numberText.setPosition(
                x + (buttonWidth - textBounds.width) / 2.0f - textBounds.left,
                y + (buttonHeight - textBounds.height) / 2.0f - textBounds.top
            );

            button.bounds = sf::FloatRect(x, y, buttonWidth, buttonHeight);
            levelButtons.push_back(button);
        }

        if (selectedLevelIndex >= static_cast<int>(levelButtons.size())) {
            selectedLevelIndex = static_cast<int>(levelButtons.size()) - 1;
        }
        if (selectedLevelIndex < 0 && !levelButtons.empty()) {
            selectedLevelIndex = 0;
        }
    }

    void LevelSelectState::navigateUp() {
        if (levelButtons.empty()) return;

        try {
            SoundManager::getInstance().playSound(SoundType::ButtonClick);
        }
        catch (...) {}

        int row = getSelectedRow();
        int col = getSelectedColumn();
        row = (row - 1 + LEVELS_PER_COLUMN) % LEVELS_PER_COLUMN;
        setSelectedPosition(row, col);
    }

    void LevelSelectState::navigateDown() {
        if (levelButtons.empty()) return;

        try {
            SoundManager::getInstance().playSound(SoundType::ButtonClick);
        }
        catch (...) {}

        int row = getSelectedRow();
        int col = getSelectedColumn();
        row = (row + 1) % LEVELS_PER_COLUMN;
        setSelectedPosition(row, col);
    }

    void LevelSelectState::navigateLeft() {
        if (levelButtons.empty()) return;

        try {
            SoundManager::getInstance().playSound(SoundType::ButtonClick);
        }
        catch (...) {}

        int row = getSelectedRow();
        int col = getSelectedColumn();
        col = (col - 1 + LEVELS_PER_ROW) % LEVELS_PER_ROW;
        setSelectedPosition(row, col);
    }

    void LevelSelectState::navigateRight() {
        if (levelButtons.empty()) return;

        try {
            SoundManager::getInstance().playSound(SoundType::ButtonClick);
        }
        catch (...) {}

        int row = getSelectedRow();
        int col = getSelectedColumn();
        col = (col + 1) % LEVELS_PER_ROW;
        setSelectedPosition(row, col);
    }

    void LevelSelectState::selectLevel() {
        if (!isValidLevelIndex(selectedLevelIndex)) return;

        int levelNumber = levelButtons[selectedLevelIndex].levelNumber;

        try {
            SoundManager::getInstance().playSound(SoundType::ButtonClick);
        }
        catch (...) {}

      
        try {
            auto gameState = std::make_unique<GameState>(engine, levelNumber);
            engine.getStateMachine().changeState(std::move(gameState));
        }
        catch (const std::exception& e) {
            
        }
    }

    void LevelSelectState::previousPage() {
        if (totalPages <= 1) return;

        try {
            SoundManager::getInstance().playSound(SoundType::ButtonClick);
        }
        catch (...) {}

        currentPage = (currentPage - 1 + totalPages) % totalPages;
        selectedLevelIndex = 0;
        setupCurrentPage();
        updatePageDisplay();
    }

    void LevelSelectState::nextPage() {
        if (totalPages <= 1) return;

        try {
            SoundManager::getInstance().playSound(SoundType::ButtonClick);
        }
        catch (...) {}

        currentPage = (currentPage + 1) % totalPages;
        selectedLevelIndex = 0;
        setupCurrentPage();
        updatePageDisplay();
    }

    void LevelSelectState::returnToMenu() {
        try {
            SoundManager::getInstance().playSound(SoundType::ButtonClick);
        }
        catch (...) {}

        engine.getStateMachine().popState();
    }

    void LevelSelectState::updateSelection() {
        for (size_t i = 0; i < levelButtons.size(); ++i) {
            bool selected = (static_cast<int>(i) == selectedLevelIndex);
            levelButtons[i].selected = selected;

            sf::Color bgColor = getButtonColor(selected);
            sf::Color borderColor = selected ? sf::Color::Yellow : sf::Color::White;

            levelButtons[i].background.setFillColor(bgColor);
            levelButtons[i].border.setOutlineColor(borderColor);
            levelButtons[i].numberText.setFillColor(selected ? sf::Color::Yellow : sf::Color::White);
        }
    }

    void LevelSelectState::updatePageDisplay() {
        std::ostringstream oss;
        oss << "Page " << (currentPage + 1) << " / " << totalPages;

        setupText(pageText, oss.str(), 0, 100, 24);
        centerTextHorizontally(pageText, 100);
        pageText.setFillColor(sf::Color::White);
    }

    void LevelSelectState::setupText(sf::Text& text, const std::string& content,
        float x, float y, int size) {
        if (!font.getInfo().family.empty()) {
            text.setFont(font);
        }

        text.setString(content);
        text.setCharacterSize(size);
        text.setFillColor(sf::Color::White);
        text.setPosition(x, y);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1);
    }

    void LevelSelectState::centerTextHorizontally(sf::Text& text, float y) {
        sf::FloatRect bounds = text.getLocalBounds();
        float windowWidth = static_cast<float>(Config::Window::WIDTH);
        text.setPosition((windowWidth - bounds.width) / 2, y);
    }

    int LevelSelectState::getSelectedRow() const {
        return selectedLevelIndex / LEVELS_PER_ROW;
    }

    int LevelSelectState::getSelectedColumn() const {
        return selectedLevelIndex % LEVELS_PER_ROW;
    }

    void LevelSelectState::setSelectedPosition(int row, int col) {
        if (levelButtons.empty()) return;

        int newIndex = row * LEVELS_PER_ROW + col;
        if (newIndex >= 0 && newIndex < static_cast<int>(levelButtons.size())) {
            selectedLevelIndex = newIndex;
        }
    }

    bool LevelSelectState::isValidLevelIndex(int index) const {
        return index >= 0 && index < static_cast<int>(levelButtons.size());
    }

    sf::Color LevelSelectState::getButtonColor(bool selected) const {
        if (selected) {
            return sf::Color(100, 100, 200); 
        }
        else {
            return sf::Color(70, 70, 140); 
        }
    }

}