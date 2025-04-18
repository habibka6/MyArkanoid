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

private:
    sf::Sprite sprite; 
    const float speed = 600.0f; 
};