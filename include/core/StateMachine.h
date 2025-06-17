#pragma once
#include <memory>
#include <stack>
#include <SFML/Graphics.hpp>
#include "State.h"
namespace Arkanoid {

    // Класс для управления стеком состояний (паттерн State Machine)
    // Обрабатывает переходы между состояниями и делегирует им вызовы
    class StateMachine {
    public:
        StateMachine() = default;
        ~StateMachine() = default;

        // Управление состояниями
        void pushState(std::unique_ptr<State> state);   // Добавляет новое состояние в стек
        void popState();                                // Удаляет текущее состояние и возобновляет предыдущее
        void changeState(std::unique_ptr<State> state); // Заменяет текущее состояние новым
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
