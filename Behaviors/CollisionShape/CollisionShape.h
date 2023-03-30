#ifndef PHYSICSSIMULATIONS_COLLISIONSHAPE_H
#define PHYSICSSIMULATIONS_COLLISIONSHAPE_H

#include <SFML/Graphics.hpp>

class CollisionShape {
    sf::RectangleShape* sprite;
    sf::RectangleShape bounds;
    sf::Vector2f size;
public:
    CollisionShape(sf::RectangleShape* obj, sf::Vector2f size)
    {
        this->sprite = obj;
        this->size = size;
        bounds.setSize(size);
        bounds.setFillColor(sf::Color(255, 255, 255, 100));
    }

    void update()
    {
        auto pos = sprite->getPosition();
        auto diff = (this->sprite->getSize() - this->size) / 2.f;
        pos += diff;
        bounds.setPosition(pos);
    }

    sf::RectangleShape* getBounds()
    {
        return &bounds;
    }
};


#endif //PHYSICSSIMULATIONS_COLLISIONSHAPE_H
