#pragma once
#include <SFML/Window.hpp>
#include "GameState.h"

class InputHandler {
public:
    // ��������� ������� ����� � ���������� ��������� ����
    void handleInput(sf::RenderWindow& window, GameState& state, float deltaTime);
};