#pragma once
// Основные настройки окна
inline constexpr unsigned WINDOW_WIDTH = 1280;  // Ширина окна
inline constexpr unsigned WINDOW_HEIGHT = 720;   // Высота окна
inline constexpr float SPATIAL_GRID_CELL_SIZE = 128.0f; // Размер ячейки сетки
// Физика мяча
inline constexpr float BASE_BALL_SPEED = 500.f; // Базовая скорость мяча

// Настройки платформы
inline constexpr float PADDLE_SPEED = 600.f; // Скорость движения платформы

// Параметры столкновений
inline constexpr float WALL_ANGLE_VARIAT = 0.05f;   // до ±0.05 радиан (~3°) шума
inline constexpr float WALL_DAMPING = 0.95f; // Замедление при ударе о стены(какая часть скорости сохранится)
inline constexpr float MIN_ANGLE_DEG = 28.f;  // Минимальный угол отскока от платформы (в градусах)
inline constexpr float MAX_ANGLE_DEG = 75.f;  // Максимальный угол отскока
inline constexpr float PADDLE_INFLUENCE = 0.3f;  // Влияние платформы на мяч (0.3 = 30%)
inline constexpr float SPEED_GROWTH = 0.0025f; // Рост скорости мяча за каждый удар (+2%)
inline constexpr float MAX_SPEED_FACTOR = 1.2f;  // Максимальная скорость (во сколько раз от базовой)
inline constexpr float MIN_VERT_RATIO = 0.30f; // Минимальная вертикальная составляющая скорости при ударе о стены
inline constexpr float BLOCK_MIN_VERT_RATIO = 0.20f; // Минимальная вертикальная составляющая при ударе о блок

    