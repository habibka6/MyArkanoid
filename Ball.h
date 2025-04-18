#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"

//����� ����(������) ��� ����
class Ball {
public:
    Ball(float startX, float startY); //������������
    void update(float dt); //���������� ������� ����
    void draw(sf::RenderWindow& window); //���������
    void reflect(const sf::Vector2f& normal); //��������� ���� �� �����������
    void reset(float x, float y); //����� ���� � ��������� ���������
    void correctPosition(const sf::FloatRect& paddleBounds, float dt); //��������� ������� ����

    //�������
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
    float getSpeed() const;
    void setVelocity(float x, float y); //��������� ��������

private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    const float baseSpeed = 600.0f; 
};