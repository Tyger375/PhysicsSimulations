#ifndef PHYSICSSIMULATIONS_OBJECT_H
#define PHYSICSSIMULATIONS_OBJECT_H


#include <SFML/Graphics.hpp>
#include "Behaviors/RigidBody/RigidBody.h"
#include "Behaviors/CollisionShape/CollisionShape.h"
#include "Vector2/Vector2.h"
#include "Behaviors/RectangleShape/RectangleShape.h"
#include "Entity.h"

class Object : public Entity {
private:
    sf::RectangleShape obj;
public:
    RectangleShape shape;
    RigidBody rb;

    Object(Vector2 size, Vector2 collisionSize, sf::Color color=sf::Color::Red) :
        shape(&obj, collisionSize),
        rb(&obj, &shape, CONTINUOUS, 1)
    {
        AddBehavior(&rb);
        AddBehavior(&shape);
        obj.setSize((sf::Vector2f) size);
        obj.setOrigin(sf::Vector2f(size.x/2, size.y/2));
        obj.setFillColor(color);
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

    void update() override
    {
        rb.update();
        shape.update();
    }

    void render(sf::RenderWindow* window) override;

    inline sf::RectangleShape getSprite() { return obj; }
    inline CollisionShape* getBounds() { return &shape; }
};


#endif //PHYSICSSIMULATIONS_OBJECT_H
