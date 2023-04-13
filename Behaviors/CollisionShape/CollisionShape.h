#ifndef PHYSICSSIMULATIONS_COLLISIONSHAPE_H
#define PHYSICSSIMULATIONS_COLLISIONSHAPE_H

#include <SFML/Graphics.hpp>
#include <valarray>
#include <iostream>
#include "../../main.h"
#include "../../Vector2/Vector2.h"
#include <string>

class CollisionShape {
    sf::RectangleShape* sprite;
    sf::RectangleShape bounds;
    Vector2 size;

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
    CollisionShape(sf::RectangleShape* obj, Vector2 size)
    {
        this->sprite = obj;
        this->size = size;
        bounds.setSize((sf::Vector2f) size);
        bounds.setOrigin(sf::Vector2f(size.x/2, size.y/2));
        bounds.setFillColor(sf::Color(255, 255, 255, 100));
    }

    bool aabbCollision(const sf::RectangleShape&);

    bool satCollision(const sf::Shape&);

    void update();

    inline sf::RectangleShape* getBounds() { return &bounds; }
};


#endif //PHYSICSSIMULATIONS_COLLISIONSHAPE_H
