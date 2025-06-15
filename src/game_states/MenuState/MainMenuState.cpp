#include "MainMenuState.h"
#include "GameState.h"
#include "OptionsState.h"
#include "LevelSelectState.h"

namespace Arkanoid {

    MainMenuState::MainMenuState(GameEngine& engine) : MenuStateBase(engine) {
        setupText(titleText, "ARKANOID", 100, 72);
        setupMenu();
    }

    void MainMenuState::setupMenu() {
        menuItems.clear();
        menuItems.push_back({ "START GAME", [this]() {
            engine.getStateMachine().changeState(std::make_unique<GameState>(engine, 1));
        } });
        menuItems.push_back({ "SELECT LEVEL", [this]() {
            engine.getStateMachine().pushState(std::make_unique<LevelSelectState>(engine));
        } });
        menuItems.push_back({ "OPTIONS", [this]() {
            engine.getStateMachine().pushState(std::make_unique<OptionsState>(engine));
        } });
        menuItems.push_back({ "EXIT", [this]() { engine.quit(); } });

        float startY = 250, spacing = 70;
        for (int i = 0; i < menuItems.size(); ++i) {
            setupText(menuItems[i].text, menuItems[i].label, startY + i * spacing);
        }
    }

}
