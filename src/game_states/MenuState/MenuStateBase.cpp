#include "MenuStateBase.h"
#include "AssetManager.h"
#include "SoundManager.h"
#include "Config.h"

namespace Arkanoid {

    MenuStateBase::MenuStateBase(GameEngine& engine)
        : State(engine)
    {
        try {
            font = AssetManager::getInstance().getFont("retro.ttf");
        }
        catch (...) {}
    }

    void MenuStateBase::enter() {

        try {
            sf::Texture& bgTexture = AssetManager::getInstance().getTexture("menu_background.jpg");
            backgroundSprite.setTexture(bgTexture);

            auto winSize = engine.getWindow().getSize();
            auto texSize = bgTexture.getSize();
            backgroundSprite.setScale(
                float(winSize.x) / texSize.x,
                float(winSize.y) / texSize.y
            );
        }
        catch (...) {}

        try {
            if (!SoundManager::getInstance().isMusicPlaying()) {
                SoundManager::getInstance().playMusic("menu_music.ogg", true);
            }
        }
        catch (...) {}

        updateHighlight();
    }

    void MenuStateBase::render(sf::RenderWindow& window) {
        if (backgroundSprite.getTexture()) {
            window.draw(backgroundSprite);
        }

        window.draw(titleText);

        for (const auto& item : menuItems) {
            window.draw(item.text);
        }
    }

    void MenuStateBase::handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
            case sf::Keyboard::Up:   navigate(-1); break;
            case sf::Keyboard::Down: navigate(1);   break;
            case sf::Keyboard::Enter:
            case sf::Keyboard::Space: onSelect(selectedItem); break;
            case sf::Keyboard::Escape: onBack(); break;
            default: break;
            }
        }

        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos(
                static_cast<float>(event.mouseMove.x),
                static_cast<float>(event.mouseMove.y)
            );

            for (int i = 0; i < menuItems.size(); ++i) {
                if (menuItems[i].text.getGlobalBounds().contains(mousePos)) {
                    selectedItem = i;
                    updateHighlight();
                    break;
                }
            }
        }

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos(
                static_cast<float>(event.mouseButton.x),
                static_cast<float>(event.mouseButton.y)
            );

            for (int i = 0; i < menuItems.size(); ++i) {
                if (menuItems[i].text.getGlobalBounds().contains(mousePos)) {
                    onSelect(i);
                    break;
                }
            }
        }
    }


    void MenuStateBase::navigate(int dir) {
        int n = static_cast<int>(menuItems.size());
        selectedItem = (selectedItem + dir + n) % n;
        updateHighlight();

        try {
            SoundManager::getInstance().playSound(SoundType::ButtonClick);
        }
        catch (...) {}
    }

    void MenuStateBase::onSelect(int idx) {
        try {
            SoundManager::getInstance().playSound(SoundType::ButtonClick);
        }
        catch (...) {}

        if (menuItems[idx].action) {
            menuItems[idx].action();
        }
    }

    void MenuStateBase::updateHighlight() {
        for (int i = 0; i < menuItems.size(); ++i) {
            bool isSelected = (i == selectedItem);
            menuItems[i].text.setFillColor(isSelected ? sf::Color::Yellow : sf::Color::White);
            menuItems[i].text.setOutlineColor(isSelected ? sf::Color::Red : sf::Color::Black);
            menuItems[i].text.setOutlineThickness(isSelected ? 2 : 1);
        }
    }

    void MenuStateBase::setupText(sf::Text& text, const std::string& str, float y, int size) {
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(size);
        centerText(text, y);
    }

    void MenuStateBase::centerText(sf::Text& text, float y) {
        auto bounds = text.getLocalBounds();
        float winW = static_cast<float>(engine.getWindow().getSize().x);
        text.setPosition((winW - bounds.width) / 2, y);
    }

} // namespace Arkanoid