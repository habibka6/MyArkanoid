#include "InputSystem.h"
#include "Paddle.h"
#include "Ball.h"
#include <algorithm>

namespace Arkanoid {

    void InputSystem::update() {
        if (!inputEnabled) {
            return;
        }

        updateKeyStates();;

        // ¬ыполн€ем команды дл€ зажатых клавиш
        for (const auto& [key, command] : keyHoldCommands) {
            if (isKeyPressed(key)) {
                command->execute();
            }
        }
    }

    void InputSystem::processEvent(const sf::Event& event) {
        if (!inputEnabled) {
            return;
        }

        switch (event.type) {
        case sf::Event::KeyPressed: {
            auto it = keyPressCommands.find(event.key.code);
            if (it != keyPressCommands.end()) {
                it->second->execute();
            }
            break;
        }
        case sf::Event::KeyReleased: {
            auto it = keyReleaseCommands.find(event.key.code);
            if (it != keyReleaseCommands.end()) {
                it->second->execute();
            }
            break;
        }
        default:
            break;
        }
    }

    void InputSystem::bindKey(sf::Keyboard::Key key, std::unique_ptr<InputCommand> command) {
        if (command) {
            keyHoldCommands[key] = std::move(command);
        }
    }

    void InputSystem::bindKeyPress(sf::Keyboard::Key key, std::unique_ptr<InputCommand> command) {
        if (command) {
            keyPressCommands[key] = std::move(command);
        }
    }

    void InputSystem::bindKeyRelease(sf::Keyboard::Key key, std::unique_ptr<InputCommand> command) {
        if (command) {
            keyReleaseCommands[key] = std::move(command);
        }
    }

    void InputSystem::unbindKey(sf::Keyboard::Key key) {
        keyHoldCommands.erase(key);
        keyPressCommands.erase(key);
        keyReleaseCommands.erase(key);
    }

    void InputSystem::unbindAllKeys() {
        keyHoldCommands.clear();
        keyPressCommands.clear();
        keyReleaseCommands.clear();
    }

    bool InputSystem::isKeyPressed(sf::Keyboard::Key key) const {
        return getKeyState(key);
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

    void InputSystem::reset() {
        previousKeyStates.clear();
        currentKeyStates.clear();
        inputEnabled = true;
    }

    void InputSystem::updateKeyStates() {
        previousKeyStates = currentKeyStates;
        currentKeyStates.clear();

        // ќбновл€ем только отслеживаемые клавиши дл€ производительности
        for (const auto& [key, _] : keyHoldCommands) {
            currentKeyStates[key] = sf::Keyboard::isKeyPressed(key);
        }
        for (const auto& [key, _] : keyPressCommands) {
            currentKeyStates[key] = sf::Keyboard::isKeyPressed(key);
        }
        for (const auto& [key, _] : keyReleaseCommands) {
            currentKeyStates[key] = sf::Keyboard::isKeyPressed(key);
        }
    }


    bool InputSystem::getKeyState(sf::Keyboard::Key key) const {
        return sf::Keyboard::isKeyPressed(key);
    }



    LambdaCommand::LambdaCommand(std::function<void()> action)
        : action(std::move(action)) {
    }

    void LambdaCommand::execute() {
        if (action) {
            action();
        }
    }

    std::unique_ptr<InputCommand> LambdaCommand::clone() const {
        return std::make_unique<LambdaCommand>(action);
    }
    

    
    LaunchBallCommand::LaunchBallCommand(Ball& ball)
        : ball(ball) {
    }

    void LaunchBallCommand::execute() {
        if (ball.getIsOnPaddle()) {
            ball.launch();
        }
    }

    std::unique_ptr<InputCommand> LaunchBallCommand::clone() const {
        return std::make_unique<LaunchBallCommand>(ball);
    }

  
    PauseGameCommand::PauseGameCommand(std::function<void()> pauseCallback)
        : callback(std::move(pauseCallback)) {
    }

    void PauseGameCommand::execute() {
        if (callback) {
            callback();
        }
    }

    std::unique_ptr<InputCommand> PauseGameCommand::clone() const {
        return std::make_unique<PauseGameCommand>(callback);
    }

    

  
    RestartLevelCommand::RestartLevelCommand(std::function<void()> restartCallback)
        : callback(std::move(restartCallback)) {
    }

    void RestartLevelCommand::execute() {
        if (callback) {
            callback();
        }
    }

    std::unique_ptr<InputCommand> RestartLevelCommand::clone() const {
        return std::make_unique<RestartLevelCommand>(callback);
    }


} 
