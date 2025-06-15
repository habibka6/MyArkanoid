#pragma once
#include <SFML/Graphics.hpp>

namespace Config {
    // ��������� ����
    namespace Window {
        constexpr  float WIDTH = 1280;
        constexpr float HEIGHT = 720;
        constexpr char* TITLE = "Arkanoid";
    }

    // ���������� ���������
    namespace Physics {

        // ��������
        constexpr float DEG_TO_RAD = 3.14159f / 180.0f;
        constexpr float PADDLE_INFLUENCE = 0.2f;
        constexpr float MAX_SPEED_FACTOR = 1.2f;
        constexpr float MIN_DIRECTSPEED_RATIO = 0.3f;
        constexpr float MAX_PADDLE_ANGLE = 75.0f;
        constexpr float MIN_PADDLE_ANGLE = 15.0f;
    }

    // ���
    namespace Ball {
        constexpr float BASE_SPEED = 650.0f;
        constexpr float SPEED_INCREASE_PER_HIT = 0.02f;
    }

    // ���������
    namespace Paddle {
        const sf::Vector2f SIZE{ 120.0f, 20.0f };
        constexpr float WIDTH = 120.0;
        constexpr float HEIGHT = 20.0;
        constexpr float BASE_SPEED = 900.0f;
        constexpr float EXPAND_FACTOR = 1.5f;
        constexpr float SHRINK_FACTOR = 0.7f; 
    }

    // �����
    namespace Block {
        constexpr float WIDTH = 90.0f; 
        constexpr float HEIGHT = 40.0f;
    }

    // ������ (PowerUps)
    namespace PowerUp {
        constexpr float FALL_SPEED = 200.0f;
        constexpr float DEFAULT_DURATION = 10.0f; 
        constexpr float SPAWN_CHANCE = 0.2f;
    }

    // ������� �������
    namespace Game {
        constexpr int LIVES = 3;
        constexpr float TARGET_FPS = 60;
    }
}