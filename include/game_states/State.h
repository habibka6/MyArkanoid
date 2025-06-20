﻿#pragma once
#include <SFML/Graphics.hpp>

namespace Arkanoid {

    class GameEngine;

    // Базовый класс для всех игровых состояний (паттерн "State")
    // Определяет интерфейс для конкретных состояний (меню, настройки, уровни, игра, окончание игры)
    class State {
    public:
        State(GameEngine& engine);
        virtual ~State() = default;

        // Основные методы состояния
        virtual void update(float deltaTime) = 0;
        virtual void render(sf::RenderWindow& window) = 0;
        virtual void handleEvent(const sf::Event& event) = 0;

        // Методы жизненного цикла
        virtual void enter() {}
        virtual void exit() {}
        virtual void pause() {}
        virtual void resume() {}

    protected:
        GameEngine& engine;
    };

} 
