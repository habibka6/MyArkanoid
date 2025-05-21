#pragma once
#include "GameState.h"
#include "InputHandler.h"
#include "Renderer.h"
#include "CollisionManager.h"
#include "SoundManager.h"

class GameEngine {
public:
    GameEngine();
    void run();

private:
    sf::RenderWindow window;
    GameState state;
    InputHandler inputHandler;
    Renderer renderer;
    CollisionManager collisionManager;
    SoundManager soundManager;

    void update(float dt);
};