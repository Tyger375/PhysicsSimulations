#include "CircleShape.h"

bool CircleShape::collidingWithRectangle(CollisionShape& m)
{
    //This object
    auto radius = bounds.getRadius();
    //Other object
    auto size = m.getSize();
    auto pos = (Vector2)m.getBounds()->getPosition() - (size / 2.f);

    //Works with every convex shape

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

bool CircleShape::aabbCollision(CollisionShape& m)
{
    auto type = m.getType();

    if (type == CIRCLE)
    {
        return collidingWithCircle(m);
    }
    else if (type == RECTANGLE)
    {
        return collidingWithRectangle(m);
    }
    return false;
}

Colliding CircleShape::satCollision(CollisionShape& m)
{
    return Colliding{};
    //TODO: reimplement
    /*auto type = m.getType();

    if (type == CIRCLE)
        return collidingWithCircle(m);
    else if (type == RECTANGLE)
    {
        return collidingWithRectangle(m);
    }

    return false;*/
}

void CircleShape::update()
{
    bounds.setRotation(this->sprite->getRotation());
    bounds.setPosition(sprite->getPosition());
}

bool CircleShape::collidingWithCircle(CollisionShape& m)
{
    auto distance = Vector2(m.getBounds()->getPosition() - bounds.getPosition());
    return
            distance.magnitude() < (m.getSize().magnitude() + bounds.getRadius());
}