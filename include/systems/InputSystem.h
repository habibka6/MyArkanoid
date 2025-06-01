#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <unordered_map>
#include <vector>

namespace Arkanoid {

    // Команда для обработки ввода (паттерн Command)
    class InputCommand {
    public:
        virtual ~InputCommand() = default;
        virtual void execute() = 0;
    };

    // Система обработки ввода
    class InputSystem {
    public:
        InputSystem() = default;
        ~InputSystem() = default;

        // Основные методы
        void update();
        void processEvent(const sf::Event& event);

        // Управление привязками клавиш (Command pattern)
        void bindKey(sf::Keyboard::Key key, std::unique_ptr<InputCommand> command);
        void bindKeyPress(sf::Keyboard::Key key, std::unique_ptr<InputCommand> command);
        void bindKeyRelease(sf::Keyboard::Key key, std::unique_ptr<InputCommand> command);
        void unbindKey(sf::Keyboard::Key key);

        // Проверка состояния клавиш
        bool isKeyPressed(sf::Keyboard::Key key) const;
        bool wasKeyJustPressed(sf::Keyboard::Key key) const;
        bool wasKeyJustReleased(sf::Keyboard::Key key) const;

        // Управление мышью
        sf::Vector2i getMousePosition(const sf::RenderWindow& window) const;
        bool isMouseButtonPressed(sf::Mouse::Button button) const;

        // Control state management
        void reset();

    private:
        // Команды для разных типов событий
        std::unordered_map<sf::Keyboard::Key, std::unique_ptr<InputCommand>> keyHoldCommands;
        std::unordered_map<sf::Keyboard::Key, std::unique_ptr<InputCommand>> keyPressCommands;
        std::unordered_map<sf::Keyboard::Key, std::unique_ptr<InputCommand>> keyReleaseCommands;

        // Состояние клавиш для отслеживания нажатий/отпускания
        std::unordered_map<sf::Keyboard::Key, bool> previousKeyStates;
        std::unordered_map<sf::Keyboard::Key, bool> currentKeyStates;

        // Вспомогательные методы
        void updateKeyStates();
        bool getKeyState(sf::Keyboard::Key key) const;
    };

    // Конкретные команды для игры
    class PaddleMoveLeftCommand : public InputCommand {
    public:
        PaddleMoveLeftCommand(class Paddle& paddle, float deltaTime);
        void execute() override;

    private:
        Paddle& paddle;
        float deltaTime;
    };

    class PaddleMoveRightCommand : public InputCommand {
    public:
        PaddleMoveRightCommand(class Paddle& paddle, float deltaTime);
        void execute() override;

    private:
        Paddle& paddle;
        float deltaTime;
    };

    class PauseGameCommand : public InputCommand {
    public:
        PauseGameCommand(std::function<void()> pauseCallback);
        void execute() override;

    private:
        std::function<void()> callback;
    };

} // namespace Arkanoid
