#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"
#include "Config.h"
// ����� ��������� ��� ����
class Paddle {
public:
    Paddle(float startX, float startY); //�����������
    void update(float dt, float windowWidth); //���������� ������� 
    void draw(sf::RenderWindow& window) const; //���������
    //�������
    sf::FloatRect getGlobalBounds() const; // ��������� ������ 
    sf::Vector2f getPosition() const; // ��������� ������� ���������
    sf::Vector2f getSize() const; // ��������� ������� ���������
    sf::Vector2f getVelocity() const { return velocity; }
    const float getSpeed() { return speed; }
    sf::Sprite getSprite() { return sprite; }
private:
    sf::Sprite sprite; 
    sf::Vector2f velocity;
    const float speed = PADDLE_SPEED;
};