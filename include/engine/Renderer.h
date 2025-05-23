#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "AssetManager.h"
#include <sstream>
class Renderer {
public:
    explicit Renderer(sf::RenderWindow& window, const GameState& state); 
    void render(float alpha);

private:
    sf::RenderWindow& window;
    const GameState& state; 
    sf::Text scoreText;
    sf::Sprite backgroundSprite;
    sf::Text gameOverText;

    void initUI();
};