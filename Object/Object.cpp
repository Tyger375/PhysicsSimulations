#include "Object.h"

void Object::render(sf::RenderWindow *window) {
    window->draw(getSprite());
}

Object::Object(Vector2 size, Vector2 collisionSize, sf::Color color) :
        shape(&obj, this, collisionSize),
        rb(&obj, this, &shape, CONTINUOUS) {
    AddBehavior(&rb);
    AddBehavior(&shape);
    obj.setSize((sf::Vector2f) size);
    obj.setOrigin((sf::Vector2f)(size / 2.f));
    obj.setFillColor(color);
}
