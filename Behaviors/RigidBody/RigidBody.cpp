#include "RigidBody.h"

void RigidBody::update() {
    auto deltaTime = GlobalVars::deltaTime;

    if (useGravity)
    {
        //Adding gravity
        this->velocity -= Vector2(0, PhysicsLaws::GravityAcceleration * deltaTime);
    }

    if (velocity == Vector2())
        return;
    auto pos = Vector2(parent->getPosition());
    //s(t) = s0 + v0 * t + 0.5 * a * t^2
    auto m = oldVelocity * deltaTime + (0.5f * this->velocity * deltaTime);
    //Transforming meters to pixels
    //1 m = 1000 px
    //pos:1 = x: 1000
    m *= 1000.f;
    pos += m;

    CollisionShape* obj;
    if (!isOnGround(parent->getPosition(), pos, 0.5f * velocity + oldVelocity, &obj) || velocity.y < 0)
    {
        parent->setPosition((sf::Vector2f) pos);
    }
    else
    {
        auto b = obj->getBounds();
        auto doneRotating = std::abs(b->getRotation() - parent->getRotation()) < 1.f;

        if (!doneRotating)
        {
            auto angle = (float) math::lerp(parent->getRotation(), b->getRotation(), deltaTime * 100);
            parent->setRotation(angle);
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
            auto angle = parent->getRotation();

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

            //std::cout << angle << acceleration << std::endl;
            velocity -= acceleration * deltaTime;
        }
    }

    oldVelocity = this->velocity;
}

#pragma region "Collision detection"

bool RigidBody::isOnGround(Vector2 oldPosition, Vector2 newPosition, Vector2 vel, CollisionShape** ref) {
    bool finalColliding = false;
    for (int i = 0; i < GlobalVars::size; i++)
    {
        auto m = GlobalVars::grounds[i];
        bool colliding = false;
        if (cdType == CollisionDetection::DISCRETE)
            colliding = checkDiscreteCollision(newPosition, *m);
        else if (cdType == CollisionDetection::CONTINUOUS)
            colliding = checkContinuousCollision(oldPosition, *m, vel);

        bool isGround = false;
        for (int k = 0; k < parent->getPointCount(); k++)
        {
            auto vertex = parent->getPoint(k);
            for (int j = 0; j < m->getBounds()->getPointCount(); j++)
            {
                auto vertex2 = m->getBounds()->getPoint(j);
                isGround = isGround || vertex.y < vertex2.y;
            }
        }
        if (colliding && isGround)
        {
            finalColliding = true;
            *ref = m;
            break;
        }
    }

    return finalColliding;
}

bool RigidBody::checkContinuousCollision(const Vector2 startPos, CollisionShape& m, Vector2 vel) {
    const float precision = 0.001f;
    const int max = (int)(1 / precision);

    auto distance = vel;
    auto sprite = parent;

    bool colliding;
    for (int i = 0; i <= max; i++) {
        float j = (float)i / (float)max;

        auto pos = startPos + (distance * j * GlobalVars::deltaTime * 1000.f);

        colliding = checkDiscreteCollision(pos, m);
        if (colliding)
        {
            sprite->setPosition((sf::Vector2f) pos);
            break;
        }
    }

    return colliding;
}

bool RigidBody::checkDiscreteCollision(Vector2 endPos, CollisionShape& m)
{
    collisionShape->getBounds()->setPosition((sf::Vector2f) endPos);
    return collisionShape->satCollision(m);
}

#pragma endregion