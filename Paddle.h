#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"

// ����� ��������� ��� ����
class Paddle {
public:
    Paddle(float startX, float startY); //�����������
    void update(float dt, float windowWidth); //���������� ������� 
    void draw(sf::RenderWindow& window); //���������
    //�������
    sf::FloatRect getGlobalBounds() const; // ��������� ������ 
    sf::Vector2f getPosition() const; // ��������� ������� ���������
    sf::Vector2f getSize() const; // ��������� ������� ���������
    sf::Vector2f getVelocity() const { return velocity; }
    float getSpeed() const { return speed; }
private:
    sf::Sprite sprite; 
    sf::Vector2f velocity;
    const float speed = 600.0f; 
};