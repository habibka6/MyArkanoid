cmake_minimum_required(VERSION 3.17)
project(MyArkanoid)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем путь к SFML (измените на ваш путь)
set(SFML_DIR "C:/C++Libraries/SFML-2.6.2/lib/cmake/SFML")

# Подключаем необходимые модули SFML
find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)

# Список исходных файлов
set(SOURCES

    main.cpp
    Ball.cpp
    Block.cpp
    GameEngine.cpp
    Paddle.cpp
    AssetManager.cpp
    Physics.cpp
)

# Список заголовочных файлов
set(HEADERS
    Ball.h
    AssetManager.h
    Block.h
    GameEngine.h
    Paddle.h
    Physics.cpp
)

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} "Physics.cpp")

# Подключаем SFML
target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

# Указываем пути для include
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
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