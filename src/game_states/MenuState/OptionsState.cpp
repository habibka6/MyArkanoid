#include "OptionsState.h"
#include "SoundManager.h"

namespace Arkanoid {

    static const std::vector<std::string> volumeLabels = { "0%", "25%", "50%", "75%", "100%" };

    OptionsState::OptionsState(GameEngine& engine) : MenuStateBase(engine) {
        setupText(titleText, "OPTIONS", 100, 70);
        setupMenu();
    }

    void OptionsState::setupMenu() {
        menuItems.clear();
        menuItems.push_back({ "Music Volume: " + volumeLabels[musicVolumeIndex], nullptr });
        menuItems.push_back({ "Sound Volume: " + volumeLabels[soundVolumeIndex], nullptr });
        menuItems.push_back({ "BACK", [this]() { onBack(); } });

        float startY = 250, spacing = 70;
        for (int i = 0; i < menuItems.size(); ++i) {
            setupText(menuItems[i].text, menuItems[i].label, startY + i * spacing);
        }
    }
    void OptionsState::onBack() { engine.getStateMachine().popState(); }

    void OptionsState::updateVolumeLabels() {
        menuItems[0].label = "Music Volume: " + volumeLabels[musicVolumeIndex];
        menuItems[1].label = "Sound Volume: " + volumeLabels[soundVolumeIndex];
        menuItems[0].text.setString(menuItems[0].label);
        menuItems[1].text.setString(menuItems[1].label);
        updateHighlight();
    }

    void OptionsState::handleEvent(const sf::Event& event) {
        MenuStateBase::handleEvent(event);

        if (event.type == sf::Event::KeyPressed) {
            if (selectedItem == 0) { 
                if (event.key.code == sf::Keyboard::Left && musicVolumeIndex > 0) {
                    musicVolumeIndex--;
                    SoundManager::getInstance().setMusicVolume(musicVolumeIndex * 25.0f);
                    updateVolumeLabels();
                    SoundManager::getInstance().playSound(SoundType::ButtonClick);
                }
                if (event.key.code == sf::Keyboard::Right && musicVolumeIndex < 4) {
                    musicVolumeIndex++;
                    SoundManager::getInstance().setMusicVolume(musicVolumeIndex * 25.0f);
                    updateVolumeLabels();
                    SoundManager::getInstance().playSound(SoundType::ButtonClick);
                }
            }
            if (selectedItem == 1) { 
                if (event.key.code == sf::Keyboard::Left && soundVolumeIndex > 0) {
                    soundVolumeIndex--;
                    SoundManager::getInstance().setSoundVolume(soundVolumeIndex * 25.0f);
                    updateVolumeLabels();
                    SoundManager::getInstance().playSound(SoundType::ButtonClick);
                }
                if (event.key.code == sf::Keyboard::Right && soundVolumeIndex < 4) {
                    soundVolumeIndex++;
                    SoundManager::getInstance().setSoundVolume(soundVolumeIndex * 25.0f);
                    updateVolumeLabels();
                    SoundManager::getInstance().playSound(SoundType::ButtonClick);
                }
            }
        }
    }

}
