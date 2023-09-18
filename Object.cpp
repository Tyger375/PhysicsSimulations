#include "Object.h"

void Object::render(sf::RenderWindow *window) {
    window->draw(getSprite());
}