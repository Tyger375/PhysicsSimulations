#ifndef PHYSICSSIMULATIONS_OBJECT_H
#define PHYSICSSIMULATIONS_OBJECT_H


#include <SFML/Graphics.hpp>
#include "Behaviors/RigidBody/RigidBody.h"
#include "Behaviors/CollisionShape/CollisionShape.h"
#include "Vector2/Vector2.h"

class Object {
private:
    sf::RectangleShape obj;
    CollisionShape shape;
public:
    RigidBody rb;

    Object(Vector2 size, Vector2 collisionSize) :
        shape(&obj, collisionSize),
        rb(&obj, &shape, CONTINUOUS, 1)
    {
        obj.setSize((sf::Vector2f) size);
        obj.setFillColor(sf::Color::Red);
    }

    void setPosition(Vector2 pos)
    {
        obj.setPosition((sf::Vector2f) pos);
        shape.update();
    }

    void setRotation(float rot)
    {
        obj.setRotation(rot);
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
