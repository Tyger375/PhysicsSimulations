#ifndef PHYSICSSIMULATIONS_ROPEMEMBER_H
#define PHYSICSSIMULATIONS_ROPEMEMBER_H

#include "../Behaviors/CircleShape/CircleShape.h"

class RopeMember
{
    sf::CircleShape shape;
    CircleShape collision;
public:
    RopeMember(Vector2 pos, float radius, sf::Color c=sf::Color::Yellow)
            : collision(&shape, radius)
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
};

#endif //PHYSICSSIMULATIONS_ROPEMEMBER_H
