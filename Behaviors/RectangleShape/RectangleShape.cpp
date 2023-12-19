#include "RectangleShape.h"

bool RectangleShape::collidingWithCircle(CollisionShape& circle)
{
    //This object
    auto size = (Vector2)bounds.getSize();
    auto pos = (Vector2)bounds.getPosition() - (size / 2.f);

    //Works with every convex shape
    auto radius = circle.getSize().magnitude();

    float closestDistance = radius * 2;
    for (int i = 0; i < bounds.getPointCount(); i++)
    {
        auto p = Vector2(bounds.getPoint(i));
        auto d = (pos - p).magnitude();
        if (d < closestDistance)
            closestDistance = d;
    }

    return closestDistance <= radius;
}

bool RectangleShape::aabbCollision(CollisionShape& m)
{
    auto type = m.getType();

    if (type == CIRCLE)
    {
        return collidingWithCircle(m);
    }
    else if (type == RECTANGLE)
    {
        //This object
        auto size = bounds.getSize();
        auto pos = bounds.getPosition() - (size / 2.f);
        auto pX = pos.x;
        auto pY = pos.y;
        auto sX = size.x;
        auto sY = size.y;

        //Other object
        auto pos2 = (Vector2)m.getBounds()->getPosition() - (m.getSize() / 2.f);
        auto p2X = pos2.x;
        auto p2Y = pos2.y;
        auto size2 = m.getSize();
        auto s2X = size2.x;
        auto s2Y = size2.y;

        return (
                pX < p2X + s2X &&
                pX + sX > p2X &&
                pY < p2Y + s2Y &&
                pY + sY > p2Y
        );
    }
    return false;
}

Colliding RectangleShape::satCollision(CollisionShape& m)
{
    auto type = m.getType();

    if (type == CIRCLE)
        //return collidingWithCircle(m);
        return Colliding{};
    else if (type == RECTANGLE)
    {
        auto *s = &this->bounds;

        //First shape
        const unsigned int length = s->getPointCount();
        Vector2 *axes = getAxes(this->bounds);

        //Second shape
        const unsigned int length2 = m.getBounds()->getPointCount();
        Vector2 *axes2 = getAxes(*m.getBounds());

        auto c1 = getCollision(axes, length, this->bounds, *m.getBounds());
        auto c2 = getCollision(axes2, length2, this->bounds, *m.getBounds());

        auto colliding = c1.collision && c2.collision;

        Colliding obj;
        obj.collision = colliding;

        if (c1.overlap.magnitude() < c2.overlap.magnitude())
        {
            obj.overlap = c1.overlap;
            obj.normal = c1.normal;
        }
        else
        {
            obj.overlap = c2.overlap;
            obj.normal = c2.normal;
        }

        if (c1.penetration > c2.penetration)
            obj.penetration = c1.penetration;
        else
            obj.penetration = c2.penetration;

        return obj;
    }

    return Colliding{false};
}

void RectangleShape::update()
{
    bounds.setRotation(this->sprite->getRotation());
    bounds.setPosition(sprite->getPosition());
}