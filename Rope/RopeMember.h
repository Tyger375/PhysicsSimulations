#ifndef PHYSICSSIMULATIONS_ROPEMEMBER_H
#define PHYSICSSIMULATIONS_ROPEMEMBER_H

#include "../Behaviors/CircleShape/CircleShape.h"

class RopeMember : public Entity
{
    sf::CircleShape shape;
    CircleShape collision;
    Mesh mesh = Mesh(&shape);
public:
    RopeMember(Vector2 pos, float radius, sf::Color c=sf::Color::Yellow)
            : collision(&mesh, radius)
    {
        shape.setRadius(radius);
        shape.setPosition((sf::Vector2f) pos);
        shape.setFillColor(c);
    }

    void setPosition(Vector2 pos)
    {
        shape.setPosition((sf::Vector2f) pos);
        collision.update();
    }
    sf::CircleShape getSprite() { return shape; }
    sf::CircleShape* pointer() { return &shape; }

    void update() override {};
    void fixedUpdate() override {};
    void beforeFixedUpdate() override {};
    void checkCollisions() override {};
    void render(sf::RenderWindow*) override {}
};

#endif //PHYSICSSIMULATIONS_ROPEMEMBER_H
