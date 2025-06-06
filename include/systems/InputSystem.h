#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <unordered_map>
#include <memory>

namespace Arkanoid {
    class InputCommand {
    public:
        virtual ~InputCommand() = default;
        virtual void execute() = 0;
        virtual std::unique_ptr<InputCommand> clone() const = 0; 
    };

    class InputSystem {
    public:
        InputSystem() = default;
        ~InputSystem() = default;

        InputSystem(const InputSystem&) = delete;
        InputSystem& operator=(const InputSystem&) = delete;
        InputSystem(InputSystem&&) = default;
        InputSystem& operator=(InputSystem&&) = default;

        // Основные методы
        void update();
        void processEvent(const sf::Event& event);

        // Управление привязками клавиш (Command pattern)
        void bindKey(sf::Keyboard::Key key, std::unique_ptr<InputCommand> command);
        void bindKeyPress(sf::Keyboard::Key key, std::unique_ptr<InputCommand> command);
        void bindKeyRelease(sf::Keyboard::Key key, std::unique_ptr<InputCommand> command);
        void unbindKey(sf::Keyboard::Key key);
        void unbindAllKeys();

        // Проверка состояния клавиш
        bool isKeyPressed(sf::Keyboard::Key key) const;
        bool wasKeyJustPressed(sf::Keyboard::Key key) const;
        bool wasKeyJustReleased(sf::Keyboard::Key key) const;

        // Управление состоянием
        void reset();
        void setEnabled(bool enabled) { inputEnabled = enabled; }
        bool isEnabled() const { return inputEnabled; }

    private:
        // Команды для разных типов событий
        std::unordered_map<sf::Keyboard::Key, std::unique_ptr<InputCommand>> keyHoldCommands;
        std::unordered_map<sf::Keyboard::Key, std::unique_ptr<InputCommand>> keyPressCommands;
        std::unordered_map<sf::Keyboard::Key, std::unique_ptr<InputCommand>> keyReleaseCommands;

        // Состояние клавиш для отслеживания нажатий/отпускания
        std::unordered_map<sf::Keyboard::Key, bool> previousKeyStates;
        std::unordered_map<sf::Keyboard::Key, bool> currentKeyStates;

        // Флаг активности системы ввода
        bool inputEnabled = true;

        // Вспомогательные методы
        void updateKeyStates();
        bool getKeyState(sf::Keyboard::Key key) const;
    };

 

    // Базовая команда с лямбда-функцией
    class LambdaCommand : public InputCommand {
    public:
        explicit LambdaCommand(std::function<void()> action);
        void execute() override;
        std::unique_ptr<InputCommand> clone() const override;

    private:
        std::function<void()> action;
    };

    // Команда запуска мяча
    class LaunchBallCommand : public InputCommand {
    public:
        LaunchBallCommand(class Ball& ball);
        void execute() override;
        std::unique_ptr<InputCommand> clone() const override;

    private:
        Ball& ball;
    };

    // Команда паузы игры
    class PauseGameCommand : public InputCommand {
    public:
        explicit PauseGameCommand(std::function<void()> pauseCallback);
        void execute() override;
        std::unique_ptr<InputCommand> clone() const override;

    private:
        std::function<void()> callback;
    };

  
    // Команда перезапуска уровня
    class RestartLevelCommand : public InputCommand {
    public:
        explicit RestartLevelCommand(std::function<void()> restartCallback);
        void execute() override;
        std::unique_ptr<InputCommand> clone() const override;

    private:
        std::function<void()> callback;
    };



} // namespace Arkanoid
