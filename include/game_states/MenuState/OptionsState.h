#pragma once
#include "MenuStateBase.h"

namespace Arkanoid {

    class OptionsState : public MenuStateBase {
    public:
        OptionsState(GameEngine& engine);

    protected:
        void setupMenu() override;
        void onBack() override;
        void handleEvent(const sf::Event& event) override;

    private:
        int musicVolumeIndex = 2;  // Текущая громкость музыки (0-4)
        int soundVolumeIndex = 3;  // Текущая громкость звуков (0-4)
        void updateVolumeLabels(); // Обновление отображения громкости
    };

}
