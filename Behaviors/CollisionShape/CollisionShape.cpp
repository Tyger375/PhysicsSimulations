#include "CollisionShape.h"

bool CollisionShape::aabbCollision(const sf::RectangleShape& m)
{
    //this object
    auto pos = bounds.getPosition();
    auto pX = pos.x;
    auto pY = pos.y;
    auto sX = size.x;
    auto sY = size.y;

    //Other object
    auto pos2 = m.getPosition();
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

bool CollisionShape::satCollision(const sf::Shape& m)
{
    auto *s = &this->bounds;

    //First shape
    const unsigned int length = s->getPointCount();
    Vector2* axes = getAxes(this->bounds);

    //Second shape
    const unsigned int length2 = m.getPointCount();
    Vector2* axes2 = getAxes(m);

    return
            getCollision(axes, length, this->bounds, m)
            &&
            getCollision(axes2, length2, this->bounds, m);
}

void CollisionShape::update()
{
    bounds.setRotation(this->sprite->getRotation());

    auto pos = sprite->getPosition();
    auto diff = (this->sprite->getSize() - (sf::Vector2f) this->size) / 2.f;
    pos += diff;
    bounds.setPosition(pos);
}