#include "InputSystem.h"
#include "Paddle.h"

namespace Arkanoid {

    // --- InputSystem Implementation ---

    void InputSystem::update() {
        updateKeyStates();

        // ¬ыполн€ем команды дл€ зажатых клавиш
        for (const auto& [key, command] : keyHoldCommands) {
            if (isKeyPressed(key)) {
                command->execute();
            }
        }
    }

    void InputSystem::processEvent(const sf::Event& event) {
        if (event.type == sf::Event::KeyPressed) {
            auto it = keyPressCommands.find(event.key.code);
            if (it != keyPressCommands.end()) {
                it->second->execute();
            }
        }
        else if (event.type == sf::Event::KeyReleased) {
            auto it = keyReleaseCommands.find(event.key.code);
            if (it != keyReleaseCommands.end()) {
                it->second->execute();
            }
        }
    }

    void InputSystem::bindKey(sf::Keyboard::Key key, std::unique_ptr<InputCommand> command) {
        keyHoldCommands[key] = std::move(command);
    }

    void InputSystem::bindKeyPress(sf::Keyboard::Key key, std::unique_ptr<InputCommand> command) {
        keyPressCommands[key] = std::move(command);
    }

    void InputSystem::bindKeyRelease(sf::Keyboard::Key key, std::unique_ptr<InputCommand> command) {
        keyReleaseCommands[key] = std::move(command);
    }

    void InputSystem::unbindKey(sf::Keyboard::Key key) {
        keyHoldCommands.erase(key);
        keyPressCommands.erase(key);
        keyReleaseCommands.erase(key);
    }

    bool InputSystem::isKeyPressed(sf::Keyboard::Key key) const {
        return sf::Keyboard::isKeyPressed(key);
    }

    bool InputSystem::wasKeyJustPressed(sf::Keyboard::Key key) const {
        auto current = currentKeyStates.find(key);
        auto previous = previousKeyStates.find(key);

        bool currentPressed = (current != currentKeyStates.end()) ? current->second : false;
        bool previousPressed = (previous != previousKeyStates.end()) ? previous->second : false;

        return currentPressed && !previousPressed;
    }

    bool InputSystem::wasKeyJustReleased(sf::Keyboard::Key key) const {
        auto current = currentKeyStates.find(key);
        auto previous = previousKeyStates.find(key);

        bool currentPressed = (current != currentKeyStates.end()) ? current->second : false;
        bool previousPressed = (previous != previousKeyStates.end()) ? previous->second : false;

        return !currentPressed && previousPressed;
    }

    sf::Vector2i InputSystem::getMousePosition(const sf::RenderWindow& window) const {
        return sf::Mouse::getPosition(window);
    }

    bool InputSystem::isMouseButtonPressed(sf::Mouse::Button button) const {
        return sf::Mouse::isButtonPressed(button);
    }

    void InputSystem::reset() {
        previousKeyStates.clear();
        currentKeyStates.clear();
    }

    void InputSystem::updateKeyStates() {
        previousKeyStates = currentKeyStates;

        // ќбновл€ем состо€ни€ всех отслеживаемых клавиш
        for (int key = 0; key < sf::Keyboard::KeyCount; ++key) {
            sf::Keyboard::Key keyCode = static_cast<sf::Keyboard::Key>(key);
            currentKeyStates[keyCode] = sf::Keyboard::isKeyPressed(keyCode);
        }
    }

    bool InputSystem::getKeyState(sf::Keyboard::Key key) const {
        return sf::Keyboard::isKeyPressed(key);
    }

    // --- Command Implementations ---

    PaddleMoveLeftCommand::PaddleMoveLeftCommand(Paddle& paddle, float deltaTime)
        : paddle(paddle), deltaTime(deltaTime) {
    }

    void PaddleMoveLeftCommand::execute() {
        paddle.move({ -paddle.getSpeed() * deltaTime, 0 });
    }

    PaddleMoveRightCommand::PaddleMoveRightCommand(Paddle& paddle, float deltaTime)
        : paddle(paddle), deltaTime(deltaTime) {
    }

    void PaddleMoveRightCommand::execute() {
        paddle.move({ paddle.getSpeed() * deltaTime, 0 });
    }

    PauseGameCommand::PauseGameCommand(std::function<void()> pauseCallback)
        : callback(pauseCallback) {
    }

    void PauseGameCommand::execute() {
        if (callback) {
            callback();
        }
    }

} // namespace Arkanoid
