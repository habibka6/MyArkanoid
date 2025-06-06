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

        // �������� ������
        void update();
        void processEvent(const sf::Event& event);

        // ���������� ���������� ������ (Command pattern)
        void bindKey(sf::Keyboard::Key key, std::unique_ptr<InputCommand> command);
        void bindKeyPress(sf::Keyboard::Key key, std::unique_ptr<InputCommand> command);
        void bindKeyRelease(sf::Keyboard::Key key, std::unique_ptr<InputCommand> command);
        void unbindKey(sf::Keyboard::Key key);
        void unbindAllKeys();

        // �������� ��������� ������
        bool isKeyPressed(sf::Keyboard::Key key) const;
        bool wasKeyJustPressed(sf::Keyboard::Key key) const;
        bool wasKeyJustReleased(sf::Keyboard::Key key) const;

        // ���������� ����������
        void reset();
        void setEnabled(bool enabled) { inputEnabled = enabled; }
        bool isEnabled() const { return inputEnabled; }

    private:
        // ������� ��� ������ ����� �������
        std::unordered_map<sf::Keyboard::Key, std::unique_ptr<InputCommand>> keyHoldCommands;
        std::unordered_map<sf::Keyboard::Key, std::unique_ptr<InputCommand>> keyPressCommands;
        std::unordered_map<sf::Keyboard::Key, std::unique_ptr<InputCommand>> keyReleaseCommands;

        // ��������� ������ ��� ������������ �������/����������
        std::unordered_map<sf::Keyboard::Key, bool> previousKeyStates;
        std::unordered_map<sf::Keyboard::Key, bool> currentKeyStates;

        // ���� ���������� ������� �����
        bool inputEnabled = true;

        // ��������������� ������
        void updateKeyStates();
        bool getKeyState(sf::Keyboard::Key key) const;
    };

 

    // ������� ������� � ������-��������
    class LambdaCommand : public InputCommand {
    public:
        explicit LambdaCommand(std::function<void()> action);
        void execute() override;
        std::unique_ptr<InputCommand> clone() const override;

    private:
        std::function<void()> action;
    };

    // ������� ������� ����
    class LaunchBallCommand : public InputCommand {
    public:
        LaunchBallCommand(class Ball& ball);
        void execute() override;
        std::unique_ptr<InputCommand> clone() const override;

    private:
        Ball& ball;
    };

    // ������� ����� ����
    class PauseGameCommand : public InputCommand {
    public:
        explicit PauseGameCommand(std::function<void()> pauseCallback);
        void execute() override;
        std::unique_ptr<InputCommand> clone() const override;

    private:
        std::function<void()> callback;
    };

  
    // ������� ����������� ������
    class RestartLevelCommand : public InputCommand {
    public:
        explicit RestartLevelCommand(std::function<void()> restartCallback);
        void execute() override;
        std::unique_ptr<InputCommand> clone() const override;

    private:
        std::function<void()> callback;
    };



} // namespace Arkanoid
