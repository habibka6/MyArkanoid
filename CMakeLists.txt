cmake_minimum_required(VERSION 3.17)
project(MyArkanoid LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Объявляем список исходных файлов
set(SOURCES
    main.cpp

    # Движок
    src/core/GameEngine.cpp
    src/core/StateMachine.cpp
    
    # Сущности
    src/entities/Ball.cpp
    src/entities/Paddle.cpp

    # Блоки
    src/entities/blocks/BaseBlock.cpp
    src/entities/blocks/Block.cpp
    src/entities/blocks/Rock.cpp

    # Бонусы
    src/entities/powerups/BasePowerUp.cpp
    src/entities/powerups/ExtraLifePowerUp.cpp
    src/entities/powerups/ExpandPaddlePowerUp.cpp
    src/entities/powerups/SlowBallPowerUp.cpp
    src/entities/powerups/ShrinkPaddlePowerUp.cpp
  

    src/entities/powerups/PowerUpEffects/ExtraLifeEffect.cpp;
    src/entities/powerups/PowerUpEffects/ExpandPaddleEffect.cpp;
    src/entities/powerups/PowerUpEffects/SlowBallEffect.cpp;
    src/entities/powerups/PowerUpEffects/ShrinkPaddleEffect.cpp;
   
    # Менеджеры систем
    src/managers/AssetManager.cpp
    src/managers/LevelManager.cpp
    src/managers/SoundManager.cpp

    # Физика и коллизии (обработчики)
    src/systems/solvers/BlockCollisionSolver.cpp  
    src/systems/solvers/WallCollisionSolver.cpp  
    src/systems/solvers/PaddleCollisionSolver.cpp  
    src/systems/SpatialGrid.cpp

    # Системы
    src/systems/PhysicsSystem.cpp
    src/systems/RenderSystem.cpp
    src/systems/InputSystem.cpp

    # Состояния игры
    src/game_states/MainMenuState.cpp
    src/game_states/GameState.cpp
    src/game_states/GameOverState.cpp
    src/game_states/State.cpp
    # Утилиты
    src/utils/MathUtils.cpp
)

# *** ВАЖНО: СОЗДАЕМ ИСПОЛНЯЕМЫЙ ФАЙЛ ***
add_executable(${PROJECT_NAME} ${SOURCES}  "include/systems/solvers/PhysicsUtils.h" "src/managers/PowerUpManager.cpp" "include/managers/PowerUpManager.h")

# Указываем путь к SFML
set(SFML_DIR "C:/C++Libraries/SFML-2.6.2/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)

# Теперь указываем include директории (ПОСЛЕ создания target)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/include/entities
    ${CMAKE_SOURCE_DIR}/include/entities/blocks
    ${CMAKE_SOURCE_DIR}/include/entities/powerups
    ${CMAKE_SOURCE_DIR}/include/entities/powerups/PowerUpEffects
    ${CMAKE_SOURCE_DIR}/include/game_states
    ${CMAKE_SOURCE_DIR}/include/managers
    ${CMAKE_SOURCE_DIR}/include/systems
    ${CMAKE_SOURCE_DIR}/include/systems
    ${CMAKE_SOURCE_DIR}/include/systems/solvers
    ${CMAKE_SOURCE_DIR}/include/utils
)

# Линкуем SFML
target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)


# Копирование DLL и ассетов
if(WIN32)
    # Список всех необходимых DLL
    set(NEEDED_DLLS
        sfml-graphics-2.dll
        sfml-graphics-d-2.dll
        sfml-window-2.dll
        sfml-window-d-2.dll
        sfml-system-2.dll
        sfml-system-d-2.dll
        sfml-audio-2.dll
        sfml-audio-d-2.dll
        openal32.dll
    )
    
    # Копируем каждый файл
    foreach(DLL ${NEEDED_DLLS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/${DLL}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endforeach()
    
    # Копирование папки assets
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    )
endif()
