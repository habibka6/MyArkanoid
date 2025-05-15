#include "Rock.h"

Rock::Rock(float x, float y)
{
    shape.setSize({ 100.f, 40.f });
    shape.setPosition(x, y);
    shape.setFillColor(sf::Color(128, 128, 128));   // серый
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Black);
}

void Rock::draw(sf::RenderWindow& wnd) { wnd.draw(shape); }
sf::FloatRect Rock::getBounds() const { return shape.getGlobalBounds(); }
bool Rock::hit() { return false; }
bool Rock::isDestroyed() const { return false; }
int  Rock::getPoints() const { return 0; }
