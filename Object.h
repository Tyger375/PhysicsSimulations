#ifndef PHYSICSSIMULATIONS_OBJECT_H
#define PHYSICSSIMULATIONS_OBJECT_H


#include <SFML/Graphics.hpp>
#include "Behaviors/RigidBody/RigidBody.h"
#include "Behaviors/CollisionShape/CollisionShape.h"

class Object {
private:
    sf::RectangleShape obj;
    RigidBody rb;
    CollisionShape shape;
public:
    Object() : shape(&obj, sf::Vector2f(120, 120)), rb(&obj, shape.getBounds())
    {
        obj.setSize(sf::Vector2f(100, 100));
        obj.setFillColor(sf::Color::Red);
    }

    void update()
    {
        rb.update();
        shape.update();
    }

    void setPosition(sf::Vector2f pos) { obj.setPosition(pos); }
    sf::RectangleShape getSprite() { return obj; }
    sf::RectangleShape debugBounds() { return *shape.getBounds(); }
};


#endif //PHYSICSSIMULATIONS_OBJECT_H
