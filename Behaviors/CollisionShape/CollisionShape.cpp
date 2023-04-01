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

void CollisionShape::update()
{
    auto pos = sprite->getPosition();
    auto diff = (this->sprite->getSize() - this->size) / 2.f;
    pos += diff;
    bounds.setPosition(pos);
}