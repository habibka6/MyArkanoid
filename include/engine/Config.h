#pragma once
// �������� ��������� ����
inline constexpr unsigned WINDOW_WIDTH = 1280;  // ������ ����
inline constexpr unsigned WINDOW_HEIGHT = 720;   // ������ ����
inline constexpr float SPATIAL_GRID_CELL_SIZE = 128.0f; // ������ ������ �����
// ������ ����
inline constexpr float BASE_BALL_SPEED = 500.f; // ������� �������� ����

// ��������� ���������
inline constexpr float PADDLE_SPEED = 600.f; // �������� �������� ���������

// ��������� ������������
inline constexpr float WALL_ANGLE_VARIAT = 0.05f;   // �� �0.05 ������ (~3�) ����
inline constexpr float WALL_DAMPING = 0.95f; // ���������� ��� ����� � �����(����� ����� �������� ����������)
inline constexpr float MIN_ANGLE_DEG = 28.f;  // ����������� ���� ������� �� ��������� (� ��������)
inline constexpr float MAX_ANGLE_DEG = 75.f;  // ������������ ���� �������
inline constexpr float PADDLE_INFLUENCE = 0.3f;  // ������� ��������� �� ��� (0.3 = 30%)
inline constexpr float SPEED_GROWTH = 0.0025f; // ���� �������� ���� �� ������ ���� (+2%)
inline constexpr float MAX_SPEED_FACTOR = 1.2f;  // ������������ �������� (�� ������� ��� �� �������)
inline constexpr float MIN_VERT_RATIO = 0.30f; // ����������� ������������ ������������ �������� ��� ����� � �����
inline constexpr float BLOCK_MIN_VERT_RATIO = 0.20f; // ����������� ������������ ������������ ��� ����� � ����

    