cmake_minimum_required(VERSION 3.17)
project(MyArkanoid LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ��������� ������ �������� ������
set(SOURCES
    main.cpp
    src/core/Ball.cpp
    src/core/Paddle.cpp
    src/core/Block.cpp
    src/core/Rock.cpp

    src/resource/AssetManager.cpp
    src/physics/BlockCollision.cpp  
    src/physics/WallCollision.cpp  
    src/physics/PaddleCollision.cpp  
    src/physics/SpatialGrid.cpp
    src/engine/GameEngine.cpp
    
    
    
    

)

# ��������� ���� � SFML
set(SFML_DIR "C:/C++Libraries/SFML-2.6.2/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)

# ��������� ����
add_executable(${PROJECT_NAME} ${SOURCES} "include/engine/GameState.h" "src/engine/GameState.cpp" "include/engine/InputHandler.h" "src/engine/InputHandler.cpp" "include/engine/Renderer.h" "src/engine/Renderer.cpp" "include/engine/CollisionManager.h" "src/engine/CollisionManager.cpp" "include/engine/SoundManager.h" "src/engine/SoundManager.cpp")

# ������ ��������� include ����������
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/include/physics
    ${CMAKE_SOURCE_DIR}/include/resource
    ${CMAKE_SOURCE_DIR}/include/engine
)

# ������� SFML
target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)


# ����������� DLL � �������
if(WIN32)
    # ������ ���� ����������� DLL
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
    
    # �������� ������ ����
    foreach(DLL ${NEEDED_DLLS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/${DLL}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endforeach()
    
    # ����������� ����� assets
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    )
endif()