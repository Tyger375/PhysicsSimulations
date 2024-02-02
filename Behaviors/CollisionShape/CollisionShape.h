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

    static Colliding getCollision(Vector2* axes, unsigned int length, const sf::Shape& first, const sf::Shape& second)
    {
        double leastOverlap = INT16_MAX;
        Vector2 leastAxis;
        double greatestOverlap = 0.f;
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
                double minOverlap = std::max(p1.x, p2.x);
                double maxOverlap = std::min(p1.y, p2.y);
                auto newOverlap = Vector2(minOverlap, maxOverlap);
                auto lO = leastOverlap;
                auto nO = abs(newOverlap.x - newOverlap.y);
                /*if (abs(lO - nO) < 0.0001) {
                    auto a = axis;
                    if (p1.y > p2.y)
                        a.y *= -1;
                    if (p1.x > p2.x)
                        a.x *= -1;

                    leastAxis += a;
                    leastAxis = leastAxis.normalize();
                }
                else */if (nO < lO) {
                    //leastOverlap = newOverlap;
                    leastOverlap = nO;
                    auto a = axis;
                    if (p1.y > p2.y)
                        a.y *= -1;
                    if (p1.x > p2.x)
                        a.x *= -1;

                    if (greatestOverlap < nO)
                        greatestOverlap = nO;

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

            Vector2 normal = edge.orthogonal();

            //Normalizing to get accurate projections
            axes[i] = normal.normalize();
        }
        return axes;
    }

    static Vector2 getProjection(Vector2 axis, Vector2* vertices, unsigned int length)
    {
        double min = axis.dot(vertices[0]);
        double max = min;
        for (int i = 1; i < length; i++) {
            double p = axis.dot(vertices[i]);
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

    static void findMinDistance(
            size_t verticesA,
            size_t verticesB,
            sf::Shape* boundsA,
            sf::Shape* boundsB,
            Vector2* contact1,
            Vector2* contact2,
            float* minDistSq,
            int* contactCount
    )
    {
        auto globalA = getGlobalVertices(*boundsA);
        auto globalB = getGlobalVertices(*boundsB);

        for (int i = 0; i < verticesA; i++) {
            auto vertexA = globalA[i];

            for (int j = 0; j < verticesB; j++) {
                auto bStart = globalB[j];
                auto bEnd = globalB[(j + 1) % verticesB];

                auto cp = math::dist(vertexA, bStart, bEnd);

                if (!cp.contains)
                    continue;

                if (abs(cp.dist - *minDistSq) < 0.0001)
                {
                    if (cp.point == *contact1) continue;

                    *contact2 = cp.point;
                    *contactCount = 2;
                }
                else if (cp.dist < *minDistSq)
                {
                    *minDistSq = cp.dist;
                    *contactCount = 1;
                    *contact1 = cp.point;
                }
            }
        }
    }

    static CollidingPoints findContactPoints(
            sf::Shape* boundsA,
            sf::Shape* boundsB
    )
    {
        auto contact1 = Vector2();
        auto contact2 = Vector2();
        auto contactCount = 0;

        auto verticesA = boundsA->getPointCount();
        auto verticesB = boundsB->getPointCount();

        float minDistSq = INT8_MAX;

        findMinDistance(
                verticesA,
                verticesB,
                boundsA,
                boundsB,
                &contact1,
                &contact2,
                &minDistSq,
                &contactCount
        );

        findMinDistance(
                verticesB,
                verticesA,
                boundsB,
                boundsA,
                &contact1,
                &contact2,
                &minDistSq,
                &contactCount
        );

        return CollidingPoints{contact1, contact2, contactCount};
    }
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
