#pragma once
#include "MenuStateBase.h"

namespace Arkanoid {

    class OptionsState : public MenuStateBase {
    public:
        OptionsState(GameEngine& engine);

    protected:
        void setupMenu() override;
        void onBack() override { engine.getStateMachine().popState(); }
        void handleEvent(const sf::Event& event) override;

    private:
        int musicVolumeIndex = 2;
        int soundVolumeIndex = 3; 
        void updateVolumeLabels();
    };

}
