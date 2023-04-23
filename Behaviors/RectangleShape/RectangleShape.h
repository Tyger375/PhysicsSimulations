#ifndef PHYSICSSIMULATIONS_RECTANGLESHAPE_H
#define PHYSICSSIMULATIONS_RECTANGLESHAPE_H

#include "../CollisionShape/CollisionShape.h"

class RectangleShape : public CollisionShape
{
private:
    sf::RectangleShape bounds;

    bool collidingWithCircle(CollisionShape& circle);
public:
    RectangleShape(sf::Shape *obj, Vector2 size) : CollisionShape(obj) {
        bounds.setSize((sf::Vector2f) size);
        bounds.setOrigin(sf::Vector2f(size.x/2, size.y/2));
        bounds.setFillColor(sf::Color(255, 255, 255, 100));
    }

    sf::RectangleShape * getBounds() override
    {
        return &bounds;
    }

    ShapeType getType() override { return RECTANGLE; }

    bool aabbCollision(CollisionShape& m) override;
    bool satCollision(CollisionShape& m) override;
    void update() override;

    inline Vector2 getSize() override { return bounds.getSize(); }
};


#endif //PHYSICSSIMULATIONS_RECTANGLESHAPE_H
