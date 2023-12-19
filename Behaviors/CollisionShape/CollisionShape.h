#ifndef PHYSICSSIMULATIONS_COLLISIONSHAPE_H
#define PHYSICSSIMULATIONS_COLLISIONSHAPE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../../Vector2/Vector2.h"
#include "../Behavior.h"

enum ShapeType
{
    CIRCLE, RECTANGLE
};

struct Colliding {
    bool collision = false;
    float penetration{};
    Vector2 overlap;
    Vector2 normal;
};

class CollisionShape : public Behavior {
protected:
    sf::Shape* sprite{};

    #pragma region SAT COLLISION DETECTION

    static Colliding getCollision(Vector2* axes, unsigned int length, const sf::Shape& first, const sf::Shape& second)
    {
        Vector2 leastOverlap = Vector2(1000000.f, 10000000.f);
        Vector2 leastAxis;
        float greatestOverlap = 0.f;
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
            else {
                float minOverlap = std::max(p1.x, p2.x);
                float maxOverlap = std::min(p1.y, p2.y);
                auto newOverlap = Vector2(minOverlap, maxOverlap);
                if (abs(leastOverlap.x - leastOverlap.y) > abs(newOverlap.x - newOverlap.y)) {
                    leastOverlap = newOverlap;
                    auto a = axis;
                    if (p1.y > p2.y)
                        a.y *= -1;
                    if (p1.x > p2.x)
                        a.x *= -1;

                    /*if (minOverlap > greatestOverlap && greatestOverlap < xOverlap)
                        greatestOverlap = xOverlap;
                    else if (yOverlap > xOverlap && greatestOverlap < yOverlap)
                        greatestOverlap = yOverlap;*/

                    leastAxis = a;
                }
            }
        }

        return Colliding{colliding, greatestOverlap, leastOverlap, leastAxis};
    }

    static Vector2* getGlobalVertices(const sf::Shape& s)
    {
        const unsigned int length = s.getPointCount();
        auto vertices = (Vector2*) malloc(sizeof (Vector2) * length);

        for (int i = 0; i < length; i++)
        {
            auto vertex = s.getPoint(i);

            auto p = s.getTransform().transformPoint(vertex);

            vertices[i] = (Vector2)p;
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
        delete vertices;
        return {min, max};
    }

    #pragma endregion
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
