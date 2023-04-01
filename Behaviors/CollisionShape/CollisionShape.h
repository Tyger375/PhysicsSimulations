#ifndef PHYSICSSIMULATIONS_COLLISIONSHAPE_H
#define PHYSICSSIMULATIONS_COLLISIONSHAPE_H

#include <SFML/Graphics.hpp>
#include <valarray>
#include <iostream>
#include "../../main.h"
#include <string>

class CollisionShape {
    sf::RectangleShape* sprite;
    sf::RectangleShape bounds;
    sf::Vector2f size;

    std::string vectorString(sf::Vector2f v) {
        return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
    }
public:
    CollisionShape(sf::RectangleShape* obj, sf::Vector2f size)
    {
        this->sprite = obj;
        this->size = size;
        bounds.setSize(size);
        bounds.setFillColor(sf::Color(255, 255, 255, 100));
    }

    bool aabbCollision(const sf::RectangleShape& m);

    void update();

    inline sf::RectangleShape* getBounds() { return &bounds; }
};


#endif //PHYSICSSIMULATIONS_COLLISIONSHAPE_H
