#include "CircleShape.h"

Colliding CircleShape::collidingWithRectangle(CollisionShape& m)
{
    return getCollisionRectangleAndCircle(m, *this);
}

bool CircleShape::aabbCollision(CollisionShape& m)
{
    auto type = m.getType();

    if (type == CIRCLE)
    {
        return collidingWithCircle(m).collision;
    }
    else if (type == RECTANGLE)
    {
        return collidingWithRectangle(m).collision;
    }
    return false;
}

Colliding CircleShape::satCollision(CollisionShape& m)
{
    auto type = m.getType();

    if (type == CIRCLE)
        return collidingWithCircle(m);
    else if (type == RECTANGLE)
    {
        return collidingWithRectangle(m);
    }

    return Colliding{};
}

void CircleShape::update()
{
    bounds.setRotation(this->sprite->getRotation());
    bounds.setPosition(sprite->getPosition());
}

Colliding CircleShape::collidingWithCircle(CollisionShape& m)
{
    auto thisPos = (Vector2)bounds.getPosition();
    auto thisRadius = bounds.getRadius();
    auto otherPos = (Vector2)m.getBounds()->getPosition();
    auto otherRadius = m.getSize().magnitude();

    auto d = (otherPos - thisPos);
    auto distance = d.magnitude();
    auto sumRadius = thisRadius + otherRadius;

    auto overlap = std::max(sumRadius - distance, 0.);

    return {distance < sumRadius, overlap, overlap, d.normalize(), {thisPos + d.normalize() * thisRadius, Vector2(), 1}};
}