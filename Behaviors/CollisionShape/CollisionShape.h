#ifndef PHYSICSSIMULATIONS_COLLISIONSHAPE_H
#define PHYSICSSIMULATIONS_COLLISIONSHAPE_H

#include <SFML/Graphics.hpp>
#include <valarray>
#include <iostream>
#include "../../Vector2/Vector2.h"
#include "../Behavior.h"
#include <string>

enum ShapeType
{
    CIRCLE, RECTANGLE
};

class CollisionShape : public Behavior {
protected:
    sf::Shape* sprite{};

    #pragma region SAT COLLISION DETECTION

    static bool getCollision(Vector2* axes, unsigned int length, const sf::Shape& first, const sf::Shape& second)
    {
        bool colliding = true;
        for (int i = 0; i < length; i++)
        {
            auto axis = axes[i];

            Vector2 p1 = getProjection(axis, getGlobalVertices(first), first.getPointCount());
            Vector2 p2 = getProjection(axis, getGlobalVertices(second), second.getPointCount());

            bool overlap = p1.x < p2.y && p2.x < p1.y;
            if (!overlap) {
                colliding = false;
                break;
            }
        }

        return colliding;
    }

    static Vector2* getGlobalVertices(const sf::Shape& s)
    {
        const unsigned int length = s.getPointCount();
        auto vertices = new Vector2[length];

        for (int i = 0; i < length; i++)
        {
            auto vertex = s.getPoint(i);

            auto p = s.getTransform().transformPoint(vertex);

            vertices[i] = p;
        }

        return vertices;
    }

    static Vector2* getAxes(const sf::Shape& s)
    {
        const unsigned int length = s.getPointCount();
        auto axes = new Vector2[length];
        auto vertices = getGlobalVertices(s);
        for (int i = 0; i < length; i++)
        {
            //Getting vertex
            auto v1 = vertices[i];
            //Getting other vertex
            auto v2 = vertices[i+1 == length ? 0 : i+1];

            Vector2 edge = v1 - v2;

            Vector2 normal = {-edge.y, edge.x};

            //Normalizing to get accurate projections
            axes[i] = normal.normalize();
        }
        return axes;
    }

    static Vector2 getProjection(Vector2 axis, Vector2* vertices, unsigned int length)
    {
        float min = axis.dot(vertices[0]);
        float max = min;
        for (int i = 1; i < length; i++) {
            float p = axis.dot(vertices[i]);
            if (p < min) {
                min = p;
            } else if (p > max) {
                max = p;
            }
        }
        return {min, max};
    }

    #pragma endregion
public:
    CollisionShape() = default;
    explicit CollisionShape(sf::Shape* obj)
    {
        this->sprite = obj;
    }

    virtual ShapeType getType() = 0;

    virtual bool aabbCollision(CollisionShape&) = 0;

    virtual bool satCollision(CollisionShape&) = 0;

    virtual inline sf::Shape* getBounds() = 0;

    virtual inline Vector2 getSize() = 0;
};


#endif //PHYSICSSIMULATIONS_COLLISIONSHAPE_H
