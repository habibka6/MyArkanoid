#pragma once
#include <SFML/Window.hpp>
#include "GameState.h"

class InputHandler {
public:
    // Обработка событий ввода и обновление состояния игры
    void handleInput(sf::RenderWindow& window, GameState& state, float deltaTime);
};