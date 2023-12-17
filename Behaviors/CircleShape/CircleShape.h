#ifndef PHYSICSSIMULATIONS_CIRCLESHAPE_H
#define PHYSICSSIMULATIONS_CIRCLESHAPE_H


#include "../CollisionShape/CollisionShape.h"

class CircleShape : public CollisionShape {
private:
    sf::CircleShape bounds;

    bool collidingWithRectangle(CollisionShape&);
    bool collidingWithCircle(CollisionShape&);
public:
    CircleShape(sf::Shape *obj, Entity* parent, float radius) : CollisionShape(obj, parent)
    {
        bounds.setRadius(radius);
        bounds.setFillColor(sf::Color(255, 255, 255, 100));
    }

    sf::CircleShape * getBounds() override { return &bounds; }

    ShapeType getType() override { return CIRCLE; }

    bool aabbCollision(CollisionShape&) override;
    Colliding satCollision(CollisionShape&) override;
    void update() override;

    inline Vector2 getSize() override { return {bounds.getRadius(), 0}; }
};


#endif //PHYSICSSIMULATIONS_CIRCLESHAPE_H
