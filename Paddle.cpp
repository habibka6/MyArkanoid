#include "Paddle.h"


Paddle::Paddle(float startX, float startY) {
    sprite.setTexture(AssetManager::GetTexture("paddle.png"));
    sprite.setScale(2.0f, 2.0f);
    sprite.setPosition(startX, startY);
}

//Обновление позиции платформы
void Paddle::update(float dt, float windowWidth) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        sprite.move(-speed * dt, 0); //Движение влево
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        sprite.move(speed * dt, 0); //Движение вправо
    }

    //Ограничение движения платформы в пределах окна
    float x = std::clamp(
        sprite.getPosition().x,
        0.0f,
        windowWidth - sprite.getGlobalBounds().width
    );
    sprite.setPosition(x, sprite.getPosition().y);
}

//Отрисовка 
void Paddle::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

//Геттеры
sf::FloatRect Paddle::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Paddle::getPosition() const {
    return sprite.getPosition();
}

sf::Vector2f Paddle::getSize() const {
    return {
        sprite.getGlobalBounds().width,
        sprite.getGlobalBounds().height
    };
}