#pragma once
#include "MenuStateBase.h"

namespace Arkanoid {

    struct LevelButton {
        int levelNumber;
        sf::Text text;
        sf::RectangleShape rect;
    };

    class LevelSelectState : public MenuStateBase {
    public:
        LevelSelectState(GameEngine& engine);

        void enter() override;
        void render(sf::RenderWindow& window) override;
        void handleEvent(const sf::Event& event) override;

    protected:
        void setupMenu() override;
        void onSelect(int idx) override;
        void onBack() override;

        // Сетка уровней
        static constexpr int ROWS = 3;
        static constexpr int COLS = 4;
        static constexpr int LEVELS = ROWS * COLS;

        std::vector<LevelButton> levelButtons;
        int selectedRow = 0;
        int selectedCol = 0;

        void updateHighlight();
        int getSelectedIndex() const { return selectedRow * COLS + selectedCol; }
    };

}
