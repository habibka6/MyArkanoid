#include "Paddle.h"


Paddle::Paddle(float startX, float startY) {
    sprite.setTexture(AssetManager::GetTexture("paddle.png"));
    sprite.setScale(2.0f, 2.0f);
    sprite.setPosition(startX, startY);
}

//���������� ������� ���������
void Paddle::update(float dt, float windowWidth) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        sprite.move(-speed * dt, 0); //�������� �����
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        sprite.move(speed * dt, 0); //�������� ������
    }

    //����������� �������� ��������� � �������� ����
    float x = std::clamp(
        sprite.getPosition().x,
        0.0f,
        windowWidth - sprite.getGlobalBounds().width
    );
    sprite.setPosition(x, sprite.getPosition().y);
}

//��������� 
void Paddle::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

//�������
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