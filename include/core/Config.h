#pragma once
#include <SFML/Graphics.hpp>

namespace Config {
    // ��������� ����
    namespace Window {
        constexpr unsigned WIDTH = 1280;
        constexpr unsigned HEIGHT = 720;
        constexpr unsigned FRAME_RATE = 60;
        constexpr char* TITLE = "Arkanoid";
        constexpr bool VSYNC_ENABLED = true;
    }

    // ���������� ���������
    namespace Physics {

        // ��������
        constexpr float WALL_DAMPING = 0.95f;
        constexpr float MIN_BOUNCE_ANGLE = 15.0f; 
        constexpr float PADDLE_INFLUENCE = 0.3f;
        constexpr float SPEED_GROWTH_FACTOR = 1.02f; 
        constexpr float MAX_SPEED_FACTOR = 1.3f;
    }

    // ���
    namespace Ball {
        constexpr float RADIUS = 12.0f;
        constexpr float BASE_SPEED = 550.0f;
        const sf::Color COLOR = sf::Color::White;

        // �������� � ����������
        constexpr float SPEED_INCREASE_PER_HIT = 0.01f; // 2% �� ����
    }

    // ���������
    namespace Paddle {
        const sf::Vector2f SIZE{ 120.0f, 20.0f };
        constexpr float WIDTH = 120.0;
        constexpr float HEIGHT = 20.0;
        constexpr float BASE_SPEED = 600.0f;
        const sf::Color COLOR = sf::Color::Cyan;
        constexpr float EXPAND_FACTOR = 1.5f; // ��� ������
        constexpr float SHRINK_FACTOR = 0.7f; // ��� �������������� ������
        constexpr float START_X = 1280 / 2 - 50; // ����� ������ ����� �������� ������ ���������
        constexpr float START_Y = 720 - 80;    // ������ �� ������� ���� ������

        // ����������������
        constexpr float BOTTOM_MARGIN = 50.0f; // ������ �� ���� ����
    }

    // �����
    namespace Block {
        const sf::Vector2f SIZE{ 90.0f, 40.0f };
        constexpr float OUTLINE_THICKNESS = 2.0f;
        const sf::Color OUTLINE_COLOR = sf::Color::Black;
        constexpr float SPACING = 15.0f;
        constexpr float DROP_CHANCE = 0.2f; // 20% ���� ��������� ������
        constexpr float WIDTH = 90.0f; 
        constexpr float HEIGHT = 40.0f;
        constexpr int BLOCKS_PER_ROW = 9; // ���������� ������ � ����
        constexpr int ROWS = 5; 
        constexpr float BLOCKS_START_Y = 80.0f;              
        // ���� ������
        namespace Regular {
            constexpr int HIT_POINTS = 1;
            constexpr int SCORE = 10;
            const sf::Color COLOR = sf::Color::Green;
        }

        namespace Tough {
            constexpr int HIT_POINTS = 2;
            constexpr int SCORE = 20;
            const sf::Color COLOR = sf::Color::Yellow;
            const sf::Color DAMAGED_COLOR = sf::Color(205, 164, 52); // �����-������
        }

        namespace Hardened {
            constexpr int HIT_POINTS = 3;
            constexpr int SCORE = 30;
            const sf::Color COLOR = sf::Color::Red;
        }

        namespace Rock {
            const sf::Color COLOR = sf::Color(128, 128, 128); // �����
            constexpr int SCORE = 0; // ������������
            constexpr bool IS_DESTRUCTIBLE = false;
        }
    }

    // ������ (PowerUps)
    namespace PowerUp {
        const sf::Vector2f SIZE{ 40.0f, 20.0f };
        constexpr float FALL_SPEED = 150.0f;
        constexpr float DEFAULT_DURATION = 10.0f; // ������ �� ���������
        constexpr float SPAWN_CHANCE = 0.3f; // 30% ���� ���������

        // ���� �������
        namespace SpeedBoost {
            const sf::Color COLOR = sf::Color::Blue;
            constexpr float MULTIPLIER = 1.5f;
            constexpr float DURATION = 10.0f;
            constexpr bool IS_TEMPORARY = true;
        }

        namespace PaddleEnlarge {
            const sf::Color COLOR = sf::Color::Magenta;
            constexpr float SCALE_MULTIPLIER = 1.5f;
            constexpr float DURATION = 15.0f;
            constexpr bool IS_TEMPORARY = true;
        }

        namespace ExtraLife {
            const sf::Color COLOR = sf::Color::Green;
            constexpr int LIVES_BONUS = 1;
            constexpr bool IS_TEMPORARY = false;
        }

        namespace MultiBall {
            const sf::Color COLOR = sf::Color::White;
            constexpr int EXTRA_BALLS = 2;
            constexpr bool IS_TEMPORARY = false;
        }

        namespace SlowDown {
            const sf::Color COLOR = sf::Color::Cyan;
            constexpr float MULTIPLIER = 0.7f;
            constexpr float DURATION = 8.0f;
            constexpr bool IS_TEMPORARY = true;
        }

        namespace PaddleShrink {
            const sf::Color COLOR = sf::Color::Red;
            constexpr float SCALE_MULTIPLIER = 0.7f;
            constexpr float DURATION = 12.0f;
            constexpr bool IS_TEMPORARY = true;
        }

        // ��� ������� �������
        namespace Laser {
            const sf::Color COLOR = sf::Color(255, 100, 100); // ������-�������
            constexpr float DURATION = 15.0f;
        }

        namespace StickyPaddle {
            const sf::Color COLOR = sf::Color::Yellow;
            constexpr float DURATION = 20.0f;
        }
    }

    // ������� �������
    namespace Game {
        constexpr int START_LIVES = 3;
        constexpr int MAX_LIVES = 9;
        constexpr int LEVELS_COUNT = 5;
        constexpr float BALL_RESPAWN_DELAY = 1.0f; // ������


        // �����
        constexpr float FIXED_TIMESTEP = 1.0f / 120.0f; // ��� ������
        constexpr float TARGET_FPS = 60;
    }
}