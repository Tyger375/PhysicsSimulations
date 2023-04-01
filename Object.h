#ifndef PHYSICSSIMULATIONS_OBJECT_H
#define PHYSICSSIMULATIONS_OBJECT_H


#include <SFML/Graphics.hpp>
#include "Behaviors/RigidBody/RigidBody.h"
#include "Behaviors/CollisionShape/CollisionShape.h"

class Object {
private:
    sf::RectangleShape obj;
    CollisionShape shape;
public:
    RigidBody rb;

    Object(sf::Vector2f size, sf::Vector2f collisionSize) :
        shape(&obj, collisionSize),
        rb(&obj, &shape, CONTINUOUS, 1)
    {
        obj.setSize(size);
        obj.setFillColor(sf::Color::Red);
    }

    void setPosition(sf::Vector2f pos)
    {
        obj.setPosition(pos);
        shape.update();
    }

    void update()
    {
        rb.update();
        shape.update();
    }

    inline sf::RectangleShape getSprite() { return obj; }
    inline sf::RectangleShape debugBounds() { return *shape.getBounds(); }
};


#endif //PHYSICSSIMULATIONS_OBJECT_H
