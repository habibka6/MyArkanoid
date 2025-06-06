#pragma once
#include <SFML/Graphics.hpp>

namespace Arkanoid {

    class GameEngine;

    class State {
    public:
        State(GameEngine& engine);
        virtual ~State() = default;

        // �������� ������ ���������
        virtual void update(float deltaTime) = 0;
        virtual void render(sf::RenderWindow& window) = 0;
        virtual void handleEvent(const sf::Event& event) = 0;

        // ������ ���������� �����
        virtual void enter() {}
        virtual void exit() {}
        virtual void pause() {}
        virtual void resume() {}

    protected:
        GameEngine& engine;
    };

} 
