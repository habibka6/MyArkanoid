#pragma once
#include <memory>
#include <stack>
#include <SFML/Graphics.hpp>
#include "State.h"
namespace Arkanoid {


    class StateMachine {
    public:
        StateMachine() = default;
        ~StateMachine() = default;

        // Управление состояниями
        void pushState(std::unique_ptr<State> state);
        void popState();
        void changeState(std::unique_ptr<State> state);
        void clearStates();

        // Основные методы
        void update(float deltaTime);
        void render(sf::RenderWindow& window);
        void handleEvent(const sf::Event& event);

        // Проверка состояний
        bool isEmpty() const;
        State* getCurrentState() const;

    private:
        std::stack<std::unique_ptr<State>> states;
    };

}
