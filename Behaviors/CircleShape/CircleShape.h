#ifndef PHYSICSSIMULATIONS_CIRCLESHAPE_H
#define PHYSICSSIMULATIONS_CIRCLESHAPE_H


#include "../CollisionShape/CollisionShape.h"
#include "../Mesh/Mesh.h"

class CircleShape : public CollisionShape {
private:
    sf::CircleShape bounds;

    Colliding collidingWithRectangle(CollisionShape&);
    Colliding collidingWithCircle(CollisionShape&);
public:
    CircleShape(Mesh *obj, float radius) : CollisionShape(obj->shape, nullptr)
    {
        bounds.setOrigin(sf::Vector2f(radius, radius));
        bounds.setRadius(radius);
        bounds.setFillColor(sf::Color(255, 255, 255, 100));
    }

    sf::CircleShape* getBounds() override { return &bounds; }

    ShapeType getType() override { return CIRCLE; }

    bool aabbCollision(CollisionShape&) override;
    Colliding satCollision(CollisionShape&) override;
    void update() override;

    inline Vector2 getSize() override { return {bounds.getRadius(), 0}; }
};


#endif //PHYSICSSIMULATIONS_CIRCLESHAPE_H
