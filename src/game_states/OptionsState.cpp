#include "OptionsState.h"
#include "MainMenuState.h"
#include "GameEngine.h"
#include "AssetManager.h"
#include "SoundManager.h"
#include "Config.h"

namespace Arkanoid {

    OptionItem::OptionItem(const std::string& label, const std::vector<std::string>& vals,
        int startIndex, std::function<void(int)> callback)
        : values(vals), currentIndex(startIndex), onChange(callback) {
        labelText.setString(label);
        if (!vals.empty() && startIndex < vals.size()) {
            valueText.setString(vals[startIndex]);
        }
    }

    OptionsState::OptionsState(GameEngine& engine)
        : State(engine), selectedOptionIndex(0), assetsLoaded(false) {
    }

    void OptionsState::enter() {
        loadAssets();
        initializeUI();
        initializeOptions();
        initializeInputBindings();
        SoundManager::getInstance().playMusic("menu_music.ogg", true);
    }

    void OptionsState::exit() {}

    void OptionsState::update(float deltaTime) {
        inputSystem.update();
        updateHighlight();
    }

    void OptionsState::render(sf::RenderWindow& window) {
        if (assetsLoaded) {
            window.draw(backgroundSprite);
        }

        window.draw(titleText);

        // Рендеринг опций
        for (const auto& option : options) {
            window.draw(option.labelText);
            window.draw(option.valueText);
        }

        window.draw(instructionText);
    }

    void OptionsState::initializeInputBindings() {
    // Навигация
    inputSystem.bindKeyPress(sf::Keyboard::Up,
        std::make_unique<LambdaCommand>([this]() {
            navigateUp();
        }));
    
    inputSystem.bindKeyPress(sf::Keyboard::Down,
        std::make_unique<LambdaCommand>([this]() {
            navigateDown();
        }));
    
    // Изменение значений
    inputSystem.bindKeyPress(sf::Keyboard::Left,
        std::make_unique<LambdaCommand>([this]() {
            changeValue(-1);
        }));
    
    inputSystem.bindKeyPress(sf::Keyboard::Right,
        std::make_unique<LambdaCommand>([this]() {
            changeValue(1);
        }));
    
    // Возврат в меню
    inputSystem.bindKeyPress(sf::Keyboard::Escape,
        std::make_unique<LambdaCommand>([this]() {
            returnToMenu();
        }));
}


    void OptionsState::handleEvent(const sf::Event& event) {
        inputSystem.processEvent(event);
        // Мышь
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            auto handleMouseClick = [this](sf::Vector2f mousePos) {
                for (size_t i = 0; i < options.size(); ++i) {
                    if (options[i].bounds.contains(mousePos)) {
                        selectedOptionIndex = static_cast<int>(i);
                        break;
                    }
                }
                };

            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                static_cast<float>(event.mouseButton.y));
            handleMouseClick(mousePos);
        }
    }

    void OptionsState::loadAssets() {
        try {
            font = AssetManager::getInstance().getFont("retro.ttf");
            sf::Texture& bgTexture = AssetManager::getInstance().getTexture("menu_background.jpg");
            backgroundSprite.setTexture(bgTexture);

            sf::Vector2u windowSize = engine.getWindow().getSize();
            sf::Vector2u textureSize = bgTexture.getSize();
            backgroundSprite.setScale(
                static_cast<float>(windowSize.x) / textureSize.x,
                static_cast<float>(windowSize.y) / textureSize.y
            );

            assetsLoaded = true;
        }
        catch (const std::exception& e) {
            assetsLoaded = false;
        }
    }

    void OptionsState::initializeUI() {
        setupText(titleText, "OPTIONS", 0, 100, 80);
        centerText(titleText, 100);
        titleText.setFillColor(sf::Color::Cyan);

        setupText(instructionText,
            "LEFT/RIGHT - Change Value    UP/DOWN - Navigate    ESC - Back",
            0, Config::Window::HEIGHT - 60, 20);
        centerText(instructionText, Config::Window::HEIGHT - 60);
        instructionText.setFillColor(sf::Color::Yellow);
    }

    void OptionsState::initializeOptions() {
        options.clear();

        // Громкость музыки
        options.emplace_back("Music Volume",
            std::vector<std::string>{"0%", "25%", "50%", "75%", "100%"},
            2, // 50% по умолчанию
            [this](int index) {
                float volume = index * 25.0f;
                SoundManager::getInstance().setMusicVolume(volume);
            });

        // Громкость звуков
        options.emplace_back("Sound Volume",
            std::vector<std::string>{"0%", "25%", "50%", "75%", "100%"},
            3, // 75% по умолчанию
            [this](int index) {
                float volume = index * 25.0f;
                SoundManager::getInstance().setSoundVolume(volume);
            });

      
        float startY = 200;
        float spacing = 60;

        for (size_t i = 0; i < options.size(); ++i) {
            float y = startY + i * spacing;

            setupText(options[i].labelText, options[i].labelText.getString(),
                Config::Window::WIDTH * 0.3f, y, 50);

            setupText(options[i].valueText, options[i].valueText.getString(),
                Config::Window::WIDTH * 0.65f, y, 50);

            options[i].bounds = sf::FloatRect(
                Config::Window::WIDTH * 0.25f, y - 10,
                Config::Window::WIDTH * 0.5f, 40
            );
        }

        updateHighlight();
    }

    void OptionsState::navigateUp() {
        SoundManager::getInstance().playSound(SoundType::ButtonClick);
        selectedOptionIndex = (selectedOptionIndex - 1 + options.size()) % options.size();
    }

    void OptionsState::navigateDown() {
        SoundManager::getInstance().playSound(SoundType::ButtonClick);
        selectedOptionIndex = (selectedOptionIndex + 1) % options.size();
    }

    void OptionsState::changeValue(int direction) {
        if (selectedOptionIndex >= 0 && selectedOptionIndex < options.size()) {
            auto& option = options[selectedOptionIndex];

            int newIndex = option.currentIndex + direction;
            if (newIndex >= 0 && newIndex < option.values.size()) {
                option.currentIndex = newIndex;
                option.valueText.setString(option.values[newIndex]);

                SoundManager::getInstance().playSound(SoundType::ButtonClick);

                if (option.onChange) {
                    option.onChange(newIndex);
                }
            }
        }
    }

    void OptionsState::updateHighlight() {
        for (size_t i = 0; i < options.size(); ++i) {
            sf::Color labelColor = (i == selectedOptionIndex) ? sf::Color::Yellow : sf::Color::White;
            sf::Color valueColor = (i == selectedOptionIndex) ? sf::Color::Green : sf::Color::Cyan;

            options[i].labelText.setFillColor(labelColor);
            options[i].valueText.setFillColor(valueColor);
        }
    }

    void OptionsState::returnToMenu() {
        SoundManager::getInstance().playSound(SoundType::ButtonClick);
        auto mainMenu = std::make_unique<MainMenuState>(engine);
        engine.getStateMachine().changeState(std::move(mainMenu));
    }

    void OptionsState::setupText(sf::Text& text, const std::string& content,
        float x, float y, int size) {
        text.setFont(font);
        text.setString(content);
        text.setCharacterSize(size);
        text.setFillColor(sf::Color::White);
        text.setPosition(x, y);
    }

    void OptionsState::centerText(sf::Text& text, float y) {
        sf::FloatRect bounds = text.getLocalBounds();
        float windowWidth = static_cast<float>(engine.getWindow().getSize().x);
        text.setPosition((windowWidth - bounds.width) / 2, y);
    }

} // namespace Arkanoid
