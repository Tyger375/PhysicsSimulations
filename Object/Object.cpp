#include "Object.h"

void Object::render(sf::RenderWindow *window) {
    window->draw(getSprite());
    auto bounds = *(sf::RectangleShape*)getBounds()->getBounds();
    bounds.setFillColor(sf::Color::Yellow);
    window->draw(bounds);
}

Object::Object(Vector2 size, Vector2 collisionSize, sf::Color color) :
        shape(&obj, this, collisionSize),
        rb(&obj, this, &shape, CONTINUOUS, 1) {
    AddBehavior(&rb);
    AddBehavior(&shape);
    obj.setSize((sf::Vector2f) size);
    obj.setOrigin(sf::Vector2f(size.x/2, size.y/2));
    obj.setFillColor(color);
}
