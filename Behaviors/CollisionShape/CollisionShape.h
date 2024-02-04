#ifndef PHYSICSSIMULATIONS_COLLISIONSHAPE_H
#define PHYSICSSIMULATIONS_COLLISIONSHAPE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Behavior.h"

enum ShapeType
{
    CIRCLE, RECTANGLE
};

struct CollidingPoints {
    Vector2 pointA;
    Vector2 pointB;
    int contactCount{};
};

struct Colliding {
    bool collision = false;
    double penetration{};
    double overlap{};
    Vector2 normal;
    CollidingPoints collidingPoints;
};

class CollisionShape : public Behavior {
protected:
    sf::Shape* sprite{};

    #pragma region SAT COLLISION DETECTION

    static Colliding getCollision(Vector2* axes, unsigned int length, const sf::Shape& first, const sf::Shape& second);
    static Vector2* getGlobalVertices(const sf::Shape& s);
    static Vector2* getAxes(const sf::Shape& s);
    static Vector2 getProjection(Vector2 axis, Vector2* vertices, unsigned int length);

    #pragma endregion

    static void findMinDistance(
            size_t verticesA,
            size_t verticesB,
            sf::Shape* boundsA,
            sf::Shape* boundsB,
            Vector2* contact1,
            Vector2* contact2,
            float* minDistSq,
            int* contactCount
    );

    static CollidingPoints findContactPoints(sf::Shape* boundsA, sf::Shape* boundsB);
public:
    CollisionShape(sf::Shape* obj, Entity* parent) :
            Behavior(parent)
    {
        this->sprite = obj;
    }

    virtual ShapeType getType() = 0;

    virtual bool aabbCollision(CollisionShape&) = 0;

    virtual Colliding satCollision(CollisionShape&) = 0;

    virtual inline sf::Shape* getBounds() = 0;

    virtual inline Vector2 getSize() = 0;
};


#endif //PHYSICSSIMULATIONS_COLLISIONSHAPE_H
