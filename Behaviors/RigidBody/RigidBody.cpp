#include "RigidBody.h"

void RigidBody::update() {
    auto deltaTime = GlobalVars::deltaTime;

    auto oldVelocity = this->velocity;

    //Adding gravity
    this->velocity -= sf::Vector2f(0, PhysicsLaws::GravityAcceleration * deltaTime);

    auto pos = parent->getPosition();
    //s(t) = s0 + v0 * t + 0.5 * a * t^2
    auto m = oldVelocity * deltaTime + (0.5f * this->velocity * deltaTime);
    //Transforming meters to pixels
    //1 m = 1000 px
    //pos:1 = x: 1000
    m *= 1000.f;
    pos += m;

    if (!isOnGround(parent->getPosition(), pos, 0.5f * velocity + oldVelocity) || velocity.y < 0)
        parent->setPosition(pos);
    else
    {
        std::cout << velocity.y << std::endl;
        if (velocity.y > 0)
            velocity.y = 0;
    }
}

#pragma region "Collision detection"

bool RigidBody::isOnGround(sf::Vector2f oldPosition, sf::Vector2f newPosition, sf::Vector2f vel) {
    bool colliding = false;
    for (int i = 0; i < GlobalVars::size; i++)
    {
        auto m = GlobalVars::grounds[i];
        if (cdType == CollisionDetection::DISCRETE)
            colliding = checkDiscreteCollision(newPosition, m);
        else if (cdType == CollisionDetection::CONTINUOUS)
            colliding = checkContinuousCollision(oldPosition, m, vel);
        if (colliding)
            break;
    }

    return colliding;
}

bool RigidBody::checkContinuousCollision(const sf::Vector2f startPos, const sf::RectangleShape& m, sf::Vector2f vel) {
    const float precision = 0.0001f;
    const int max = (int)(1 / precision);
    //auto distance = sf::Vector2f(x, y);
    auto distance = vel;
    auto sprite = parent;
    auto bounds = collisionShape->getBounds();

    bool colliding = false;
    for (int i = 0; i <= max; i++) {
        float j = (float)i / (float)max;
        //std::cout << "continuous checking: " << vectorString(distance * j) << std::endl;
        auto pos = startPos + (distance * j * GlobalVars::deltaTime * 1000.f);
        auto difference = (sprite->getSize() - bounds->getSize()) / 2.f;

        colliding = checkDiscreteCollision(pos + difference, m);
        if (colliding)
        {
            sprite->setPosition(pos);
            break;
        }
    }

    return colliding;
}

bool RigidBody::checkDiscreteCollision(sf::Vector2f endPos, const sf::RectangleShape& m)
{
    collisionShape->getBounds()->setPosition(endPos);
    return collisionShape->aabbCollision(m);
}

#pragma endregion