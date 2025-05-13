#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"

//����� ����(������) ��� ����
class Ball {
public:
    Ball(float startX, float startY); //������������
    void update(float dt); //���������� ������� ����
    void draw(sf::RenderWindow& window,float alpha); //���������
    void reflect(const sf::Vector2f& normal); //��������� ���� �� �����������
    void reset(float x, float y); //����� ���� � ��������� ���������
    void correctPosition(const sf::FloatRect& paddleBounds, float dt); //��������� ������� ����
   
    //�������
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
    sf::Sprite& getSprite() { return sprite; }

    
    float getSpeed() const;
    void setVelocity(float x, float y); //��������� ��������
    void savePrevPosition() {
        m_prevPosition = sprite.getPosition();
    }
private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    const float baseSpeed = 500.0f; 
    sf::Vector2f m_prevPosition; // ���������� ������� ��� ������������
};