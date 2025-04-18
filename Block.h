#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.h"

//����� ������(����������) ��� ����
class Block {
public:
    enum class Type { Green, Yellow, Red }; //���� ������
    Block(float x, float y, Type type); //�����������
    void draw(sf::RenderWindow& window); //��������� �����
    sf::FloatRect getBounds() const; //��������� ������ �����
    bool isDestroyed() const; //��������, ��������� �� ����
    bool hit(); //��������� ��������� �� �����
    int getPoints() const; //��������� ����� �� ����

private:
    void updateColor();//���������� ����� ����� � ����������� �� HP
    sf::RectangleShape shape;
    Type blockType;
    int maxHealth; //HP ����� � ������ ������
    int currentHealth; //HP ����� � ��������� �������
    int points; //���� �� ����
    bool destroyed; //���� ����������
};