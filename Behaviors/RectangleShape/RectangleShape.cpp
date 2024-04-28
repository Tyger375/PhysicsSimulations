#include "RectangleShape.h"

Colliding RectangleShape::collidingWithCircle(CollisionShape& circle)
{
    return getCollisionRectangleAndCircle(*this, circle);
}

bool RectangleShape::aabbCollision(CollisionShape& m)
{
    auto type = m.getType();

    if (type == CIRCLE)
    {
        return collidingWithCircle(m).collision;
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
        return collidingWithCircle(m);
    else if (type == RECTANGLE)
    {
        auto *s = &this->bounds;
        auto otherBounds = m.getBounds();

        //First shape
        const unsigned int length = s->getPointCount();
        Vector2 *axes = getAxes(this->bounds);

        //Second shape
        const unsigned int length2 = otherBounds->getPointCount();
        Vector2 *axes2 = getAxes(*otherBounds);

        Colliding obj;
        obj.collidingPoints = findContactPoints(s, otherBounds);

        auto c1 = getCollision(axes, length, this->bounds, *otherBounds);
        auto c2 = getCollision(axes2, length2, this->bounds, *otherBounds);

        auto colliding = c1.collision && c2.collision;

        obj.collision = colliding;

        obj.overlap = c1.overlap;
        obj.normal = c1.normal;

        //Resolve normal when there's only 1 colliding point
        if (colliding && obj.collidingPoints.contactCount == 1) {
            auto vertices = getGlobalVertices(*s);
            auto l = s->getPointCount();

            auto index = -1;

            for (int j = 0; j < s->getPointCount(); ++j) {
                auto v = vertices[j];
                auto p = obj.collidingPoints.pointA;
                auto error = 5;
                auto x = abs(v.x - p.x) < error;
                auto y = abs(v.y - p.y) < error;
                if (x && y)
                    index = j;
            }

            if (index >= 0) {
                auto p1 = vertices[index];

                auto p2 = vertices[(index - 1) < 0 ? (l - 1) : index - 1];

                auto ax1 = (p2 - p1).normalize();

                p2 = vertices[(index + 1) >= l ? 0 : index + 1];
                auto ax2 = (p1 - p2).normalize();

                ax1 = ax1.orthogonal();
                ax2 = ax2.orthogonal();

                auto v = ax1 + ax2;
                obj.normal = v.normalize();
            }
        }

        obj.penetration = c1.penetration;

        return obj;
    }

    return Colliding{false};
}

void RectangleShape::update()
{
    bounds.setRotation(sprite->getRotation());
    bounds.setPosition(sprite->getPosition());
}