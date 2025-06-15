#pragma once
#include "MenuStateBase.h"

namespace Arkanoid {

class MainMenuState : public MenuStateBase {
public:
    MainMenuState(GameEngine& engine);
protected:
    void setupMenu() override;
    void onBack() override { engine.quit(); }
};

}
