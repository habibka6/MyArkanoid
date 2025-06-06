#pragma once
#include <SFML/Graphics.hpp>

namespace Config {
    // Настройки окна
    namespace Window {
        constexpr unsigned WIDTH = 1280;
        constexpr unsigned HEIGHT = 720;
        constexpr unsigned FRAME_RATE = 60;
        constexpr char* TITLE = "Arkanoid";
        constexpr bool VSYNC_ENABLED = true;
    }

    // Физические константы
    namespace Physics {

        // Коллизии
        constexpr float DEG_TO_RAD = 3.14159f / 180.0f;
        constexpr float WALL_DAMPING = 0.95f;
        constexpr float MIN_BOUNCE_ANGLE = 15.0f; 
        constexpr float PADDLE_INFLUENCE = 0.3f;
        constexpr float MAX_SPEED_FACTOR = 1.3f;
        constexpr float MIN_VERTICAL_RATIO = 0.3f;
        constexpr float DEFAULT_SEPARATION = 2.0f;
        constexpr float MAX_PADDLE_ANGLE = 75.0f;
    }

    // Мяч
    namespace Ball {
        constexpr float RADIUS = 12.0f;
        constexpr float BASE_SPEED = 550.0f;
        const sf::Color COLOR = sf::Color::White;

        // Скорость и управление
        constexpr float SPEED_INCREASE_PER_HIT = 0.01f;
    }

    // Платформа
    namespace Paddle {
        const sf::Vector2f SIZE{ 120.0f, 20.0f };
        constexpr float WIDTH = 120.0;
        constexpr float HEIGHT = 20.0;
        constexpr float BASE_SPEED = 900.0f;
        const sf::Color COLOR = sf::Color::Cyan;
        constexpr float EXPAND_FACTOR = 1.5f;
        constexpr float SHRINK_FACTOR = 0.7f; 
        constexpr float START_X = 1280 / 2 - 50; 
        constexpr float START_Y = 720 - 80;    

        // Позиционирование
        constexpr float BOTTOM_MARGIN = 50.0f; 
    }

    // Блоки
    namespace Block {
        const sf::Vector2f SIZE{ 90.0f, 40.0f };
        constexpr float OUTLINE_THICKNESS = 2.0f;
        const sf::Color OUTLINE_COLOR = sf::Color::Black;
        constexpr float SPACING = 15.0f;
        constexpr float DROP_CHANCE = 0.2f; 
        constexpr float WIDTH = 90.0f; 
        constexpr float HEIGHT = 40.0f;
                 
        // Типы блоков
        namespace Regular {
            constexpr int HIT_POINTS = 1;
            constexpr int SCORE = 10;
            const sf::Color COLOR = sf::Color::Green;
        }

        namespace Tough {
            constexpr int HIT_POINTS = 2;
            constexpr int SCORE = 20;
            const sf::Color COLOR = sf::Color::Yellow;
            const sf::Color DAMAGED_COLOR = sf::Color(205, 164, 52);
        }

        namespace Hardened {
            constexpr int HIT_POINTS = 3;
            constexpr int SCORE = 30;
            const sf::Color COLOR = sf::Color::Red;
        }

        namespace Rock {
            const sf::Color COLOR = sf::Color(128, 128, 128); 
            constexpr int SCORE = 0; 
            constexpr bool IS_DESTRUCTIBLE = false;
        }
    }

    // Бонусы (PowerUps)
    namespace PowerUp {
        const sf::Vector2f SIZE{ 40.0f, 20.0f };
        constexpr float FALL_SPEED = 150.0f;
        constexpr float DEFAULT_DURATION = 10.0f; 
        constexpr float SPAWN_CHANCE = 0.3f;
    }

    // Игровой процесс
    namespace Game {
        constexpr int START_LIVES = 3;
        constexpr int MAX_LIVES = 9;
        constexpr int LEVELS_COUNT = 5;
        constexpr float BALL_RESPAWN_DELAY = 1.0f; 


        // Время
        constexpr float FIXED_TIMESTEP = 1.0f / 120.0f; 
        constexpr float TARGET_FPS = 60;
    }
}