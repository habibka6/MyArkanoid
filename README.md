# MyArkanoid

MyArkanoid — реализация классического Arkanoid на C++ и SFML.  
Проект построен на принципах модульности, чистой архитектуры и расширяемости.  
**В игре реализованы: система бонусов (паттерн Factory), отдельная система физики и коллизий (паттерн Observer), управление игровыми состояниями (паттерн State), менеджеры ресурсов (Singleton), гибкая работа с уровнями!**

---

## 📸 Скриншот

[Геймплей](gameplay_screenshots/gameplay_1.png)

---

## 🚀 Быстрый старт

```bash
git clone https://github.com/habibka6/MyArkanoid.git
cd MyArkanoid
mkdir build && cd build
cmake ..
cmake --build .
```

**Запуск:**
- Linux/MacOS: `./MyArkanoid`
- Windows: `MyArkanoid.exe`

---

## 🎮 Управление

- ← → — движение платформы  
- Пробел — запуск мяча  
- P — пауза  
- ESC — выход в меню/из игры  
- Enter — следующий уровень (после победы)

---

<details>
<summary><strong>📂 Структура проекта (нажми, чтобы раскрыть)</strong></summary>

```plaintext
MyArkanoid/
├── assets/                
│   ├── fonts/             
│   ├── images/              
│   ├── levels/            
│   ├── music/               
│   ├── sounds/              
│   └── textures/       
│
├── include/                 # Заголовочные файлы
│   ├── core/                # Ядро движка (GameEngine, StateMachine)
│   ├── entities/            # Игровые сущности
│   │   ├── Ball.h
│   │   ├── Entity.h
│   │   ├── MovableEntity.h
│   │   ├── Paddle.h
│   │   └── blocks/
│   │       ├── BaseBlock.h
│   │       ├── Block.h
│   │       └── Rock.h
│   │   └── PowerUpEffects/
│   │       ├── ExtraLifeEffect.h
│   │       ├── PowerUpEffect.h
│   │       ├── ScalePaddleEffect.h
│   │       └── SlowBallEffect.h
│   │   ├── ExpandPaddlePowerUp.h
│   │   ├── ExtraLifePowerUp.h
│   │   ├── PowerUp.h
│   │   ├── PowerUpFactory.h
│   │   ├── ShrinkPaddlePowerUp.h
│   │   └── SlowBallPowerUp.h
│
│   ├── game_states/         # Состояния игры
│   │   ├── GameOverState.h
│   │   ├── GameState.h
│   │   ├── State.h
│   │   └── MenuState/
│   │       ├── LevelSelectState.h
│   │       ├── MainMenuState.h
│   │       ├── MenuStateBase.h
│   │       └── OptionsState.h
│
│   ├── managers/            # Менеджеры ресурсов и логики
│   │   ├── AssetManager.h
│   │   ├── LevelManager.h
│   │   ├── PowerUpManager.h
│   │   └── SoundManager.h
│
│   ├── nlohmann/            # Внешние библиотеки (json)
│
│   └── systems/            
│       ├── ICollisionObserver.h
│       ├── PhysicsSystem.h
│       ├── RenderSystem.h
│       └── solvers/         # Солверы коллизий
│           ├── BlockCollisionSolver.h
│           ├── PaddleCollisionSolver.h
│           └── WallCollisionSolver.h
│
├── src/                     # Исходный код (.cpp)
├── main.cpp                 # Точка входа
├── CMakeLists.txt           # Файл сборки CMake
├── LICENSE.txt              # Лицензия
└── README.md               

```

- **core/** — ядро движка (GameEngine, StateMachine)
- **entities/** — игровые объекты (Ball, Paddle, Block, PowerUp и др.)
- **game_states/** — состояния игры (меню, игра, пауза и др.)
- **managers/** — менеджеры ресурсов, уровней, бонусов, звука
- **systems/** — физика, коллизии, рендер

</details>

---

## 🏆 Особенности

- Модульная архитектура, легко расширять
- Кастомные уровни (JSON)
- Управление игровыми состояниями (паттерн State + StateMachine )
- Система бонусов (паттерн Factory)
- Отдельная система физики и коллизий (Observer)
- Менеджеры ресурсов и звука (Singleton)
- Плавная анимация, музыка, эффекты

---
## 💎 Система бонусов
В игре реализована система бонусов, которые выпадают из разрушенных блоков. На данный момент существует 4 типа бонусов, различающихся по цвету и эффекту:

- 🟥 **Красный**: **Дополнительная жизнь**  
  Увеличивает количество жизней игрока
- 🟦 **Синий**: **Замедление мяча**  
  Временно снижает скорость мяча
- 🟨 **Желтый**: **Уменьшение платформы**  
  Временно уменьшает размер платформы
- 🟩  **Зеленый**: **Увеличение платформы**  
  Временно увеличивает размер платформы

---

## ⚙️ Настройка параметров игры
Все ключевые параметры игры можно гибко настраивать через файл `include/core/Config.h`:

```cpp
// Пример настроек в Config.h
namespace Config {

    constexpr float Ball::BASE_SPEED = 650.0f;
    constexpr float PADDLE_SPEED = 900.0f;
    constexpr float POWERUP::FALL_SPEED = 200.0f;
   
    constexpr float Ball::SPEED_INCREASE_PER_HIT = 0.02f; //ускорение мяча на
    constexpr int PoowerUp::SPAWN_CHANCE = 30;    // Шанс выпадения бонуса (1-100%)
    // Системные
    onstexpr float Game::TARGET_FPS = 120; 
}
```

**Что можно настроить:**
- Скорость мяча (`Ball::BASE_SPEED`)
- Скорость движения платформы (`Paddle::BASE_SPEED`)
- Скорость падения бонусов (`PowerUp::FALL_SPEED`)
- Коэффициент ускорения мяча после отскока (Ball::SPEED_INCREASE_PER_HIT = 0.02f;)
- Шанс выпадения бонусов из блоков (`PowerUp::SPAWN_CHANCE`)
- Ограничение FPS (`TARGET_FPS`)
- Размеры игровых объектов


После изменения параметров нужно пересобрать проект.

---
## 🛠️ Как добавить свой уровень

1. Создай файл в `assets/levels/`
2. Опиши уровень в формате JSON (см. примеры)
3. Новый уровень появится в игре автоматически

---
## 📦 Зависимости

- **SFML** — графика, звук, ввод
- **nlohmann/json** — парсинг JSON
- **C++17** или новее

---

## 📄 Лицензия

MIT License.  
См. файл `LICENSE.txt`.

---

## 💡 Авторы

- [habibka6](https://github.com/habibka6) — автор и разработчик

---

## 🤝 Благодарности

- SFML Community
- nlohmann/json
