#include "SoundManager.h"

SoundManager::SoundManager() {
    hitPaddleSound.setBuffer(AssetManager::getInstance().GetSoundBuffer("paddle_hit.wav"));
    hitBlockSound.setBuffer(AssetManager::getInstance().GetSoundBuffer("block_hit.wav"));
    hitRockSound.setBuffer(AssetManager::getInstance().GetSoundBuffer("rock_hit.wav"));
    loseBallSound.setBuffer(AssetManager::getInstance().GetSoundBuffer("lose_ball.wav"));
}