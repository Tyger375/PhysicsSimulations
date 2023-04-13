#include "RigidBody.h"

void RigidBody::update() {
    auto deltaTime = GlobalVars::deltaTime;

    auto oldVelocity = this->velocity;

    //Adding gravity
    this->velocity -= Vector2(0, PhysicsLaws::GravityAcceleration * deltaTime);

    auto pos = Vector2(parent->getPosition());
    //s(t) = s0 + v0 * t + 0.5 * a * t^2
    auto m = oldVelocity * deltaTime + (0.5f * this->velocity * deltaTime);
    //Transforming meters to pixels
    //1 m = 1000 px
    //pos:1 = x: 1000
    m *= 1000.f;
    pos += m;

    sf::RectangleShape obj;
    if (!isOnGround(parent->getPosition(), pos, 0.5f * velocity + oldVelocity, &obj) || velocity.y < 0)
    {
        parent->setPosition((sf::Vector2f) pos);
    }
    else
    {
        elapsedTime += deltaTime;

        auto doneRotating = std::abs(obj.getRotation() - parent->getRotation()) < 1.f;

        if (!doneRotating)
        {
            auto lerpDuration = velocity.y / 2;
            parent->setRotation(math::lerp(parent->getRotation(), obj.getRotation(), elapsedTime / lerpDuration));
        }
        if (velocity.x != 0)
        {
            pos.y = parent->getPosition().y;
            parent->setPosition((sf::Vector2f) pos);
        }
        else if (velocity.y > 0)
        {
            velocity = sf::Vector2f();
        }

        if (doneRotating)
        {
            //Inclined plane
            auto angle = obj.getRotation();
            auto w = (float) math::degToRad(angle);
            auto Fp = mass * PhysicsLaws::GravityAcceleration;
            auto FParallel = Fp * std::sin(w);
            auto FOrthogonal = Fp * std::cos(w);
            auto Fa = FOrthogonal * Fc;

            if (Fa < FParallel) //was Fa > FParallel, but we're working with negative values
                return;
            auto alpha = 90 - angle;
            auto wAlpha = (float) math::degToRad(alpha);

            auto ParallelAcceleration = FParallel / mass;
            auto acceleration = Vector2(
                    ParallelAcceleration * std::sin(wAlpha),
                    ParallelAcceleration * std::cos(wAlpha)
                    );

            velocity -= acceleration * deltaTime;
        }
    }
}

#pragma region "Collision detection"

bool RigidBody::isOnGround(Vector2 oldPosition, Vector2 newPosition, Vector2 vel, sf::RectangleShape* ref) {
    bool finalColliding = false;
    for (int i = 0; i < GlobalVars::size; i++)
    {
        auto m = GlobalVars::grounds[i];
        bool colliding = false;
        if (cdType == CollisionDetection::DISCRETE)
            colliding = checkDiscreteCollision(newPosition, m);
        else if (cdType == CollisionDetection::CONTINUOUS)
            colliding = checkContinuousCollision(oldPosition, m, vel);

        bool isGround = parent->getPosition().y < m.getPosition().y;
        if (colliding && isGround)
        {
            finalColliding = true;
            *ref = m;
            break;
        }
    }

    return finalColliding;
}

bool RigidBody::checkContinuousCollision(const Vector2 startPos, const sf::RectangleShape& m, Vector2 vel) {
    const float precision = 0.0001f;
    const int max = (int)(1 / precision);

    auto distance = vel;
    auto sprite = parent;
    auto bounds = collisionShape->getBounds();

    bool colliding = false;
    for (int i = 0; i <= max; i++) {
        float j = (float)i / (float)max;

        auto pos = startPos + (distance * j * GlobalVars::deltaTime * 1000.f);
        auto difference = (sprite->getSize() - bounds->getSize()) / 2.f;

        colliding = checkDiscreteCollision(pos + difference, m);
        if (colliding)
        {
            sprite->setPosition((sf::Vector2f) pos);
            break;
        }
    }

    return colliding;
}

bool RigidBody::checkDiscreteCollision(Vector2 endPos, const sf::RectangleShape& m)
{
    collisionShape->getBounds()->setPosition((sf::Vector2f) endPos);
    return collisionShape->satCollision(m);
}

#pragma endregion