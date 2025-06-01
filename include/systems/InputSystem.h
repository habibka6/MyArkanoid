#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <unordered_map>
#include <vector>

namespace Arkanoid {

    // ������� ��� ��������� ����� (������� Command)
    class InputCommand {
    public:
        virtual ~InputCommand() = default;
        virtual void execute() = 0;
    };

    // ������� ��������� �����
    class InputSystem {
    public:
        InputSystem() = default;
        ~InputSystem() = default;

        // �������� ������
        void update();
        void processEvent(const sf::Event& event);

        // ���������� ���������� ������ (Command pattern)
        void bindKey(sf::Keyboard::Key key, std::unique_ptr<InputCommand> command);
        void bindKeyPress(sf::Keyboard::Key key, std::unique_ptr<InputCommand> command);
        void bindKeyRelease(sf::Keyboard::Key key, std::unique_ptr<InputCommand> command);
        void unbindKey(sf::Keyboard::Key key);

        // �������� ��������� ������
        bool isKeyPressed(sf::Keyboard::Key key) const;
        bool wasKeyJustPressed(sf::Keyboard::Key key) const;
        bool wasKeyJustReleased(sf::Keyboard::Key key) const;

        // ���������� �����
        sf::Vector2i getMousePosition(const sf::RenderWindow& window) const;
        bool isMouseButtonPressed(sf::Mouse::Button button) const;

        // Control state management
        void reset();

    private:
        // ������� ��� ������ ����� �������
        std::unordered_map<sf::Keyboard::Key, std::unique_ptr<InputCommand>> keyHoldCommands;
        std::unordered_map<sf::Keyboard::Key, std::unique_ptr<InputCommand>> keyPressCommands;
        std::unordered_map<sf::Keyboard::Key, std::unique_ptr<InputCommand>> keyReleaseCommands;

        // ��������� ������ ��� ������������ �������/����������
        std::unordered_map<sf::Keyboard::Key, bool> previousKeyStates;
        std::unordered_map<sf::Keyboard::Key, bool> currentKeyStates;

        // ��������������� ������
        void updateKeyStates();
        bool getKeyState(sf::Keyboard::Key key) const;
    };

    // ���������� ������� ��� ����
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
