#include "Mesh.h"

Mesh Mesh::RectangleMesh(Vector2 size, sf::Color color) {
    auto* shape = new sf::RectangleShape();
    shape->setSize((sf::Vector2f)size);
    shape->setFillColor(color);

    return Mesh(shape);
}

Mesh Mesh::CircleMesh(float radius, sf::Color color) {
    auto* shape = new sf::CircleShape();
    shape->setOrigin(sf::Vector2f(radius, radius));
    shape->setRadius(radius);
    shape->setFillColor(color);

    return Mesh(shape);
}