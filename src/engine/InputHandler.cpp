#include "InputHandler.h"

void InputHandler::handleInput(sf::RenderWindow& window, GameState& state, float deltaTime) {
    sf::Event event;
    
    // Обработка событий окна (закрытие, ESC)
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed || 
            (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
            window.close();
        }
    }

    // Движение платформы влево/вправо
    float moveSpeed = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        moveSpeed = -state.getPaddle().getSpeed() * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        moveSpeed = state.getPaddle().getSpeed() * deltaTime;
    }

    // Запуск мяча при нажатии пробела
    if (state.waitingLaunch && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        state.launchBall();
    }
}