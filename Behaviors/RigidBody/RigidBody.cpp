#include "RigidBody.h"

void RigidBody::update() {
    auto deltaTime = GlobalVars::fixedDeltaTime;
    //auto deltaTime = 1 / 40.f;

    if (useGravity)
    {
        //Adding gravity
        this->velocity -= Vector2(0, PhysicsLaws::GravityAcceleration * deltaTime);
    }

    checkCollisions((Vector2)parent->getPosition());

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

    parent->setPosition((sf::Vector2f) pos);

    /*
     * TODO: Reimplement inclined plane and friction
    if (!isOnGround((Vector2)parent->getPosition(), pos, 0.5f * velocity + oldVelocity, &obj))
    {

    }
    else
    {
        auto b = obj->getBounds();
        auto doneRotating = std::abs(b->getRotation() - parent->getRotation()) < 1.f;

        if (!doneRotating)
        {
            auto angle = (float) math::lerp(parent->getRotation(), b->getRotation(), deltaTime * 5);
            parent->setRotation(angle);
        }

        auto angle = parent->getRotation();

        if (velocity.y > 0 && angle == 0)
        {
            velocity.y = 0.f;
        }

        if (doneRotating)
        {
            //Inclined plane
            auto w = (float) math::degToRad(angle);
            auto Fp = mass * PhysicsLaws::GravityAcceleration;
            auto FParallel = Fp * std::sin(w);
            auto FOrthogonal = Fp * std::cos(w);
            auto Fa = FOrthogonal * Fc;

            if (Fa < FParallel) //was Fa > FParallel, but we're working with negative values
            {
                if (abs(velocity.x) < 0.1)
                {
                    velocity.x = 0.f;
                }
                else
                {
                    auto acceleration = Fa / mass;
                    auto sign = math::sign(velocity.x);
                    if (abs(acceleration) > velocity.x)
                        acceleration = sign * -1 * velocity.x;
                    velocity.x += acceleration * deltaTime;
                }
            }
            else
            {
                auto alpha = 90 - angle;
                auto wAlpha = (float) math::degToRad(alpha);

                auto ParallelAcceleration = FParallel / mass;
                auto acceleration = Vector2(
                        ParallelAcceleration * std::sin(wAlpha),
                        ParallelAcceleration * std::cos(wAlpha)
                );
                auto frictionAcceleration = Fa / mass;
                auto fAcceleration = Vector2(
                        frictionAcceleration * std::sin(wAlpha),
                        frictionAcceleration * std::cos(wAlpha)
                );

                velocity -= acceleration * deltaTime;
            }
        }
        m = oldVelocity * deltaTime + (0.5f * this->velocity * deltaTime);
        m *= 1000.f;
        pos += m;
        std::cout << pos << std::endl;
        parent->setPosition((sf::Vector2f) pos);
        if (velocity.x != 0)
        {
            //pos.y = parent->getPosition().y;
        }
    }*/

    oldVelocity = this->velocity;
}

void RigidBody::checkCollisions(Vector2 newPosition) {
    for (auto & entity : GlobalVars::entities)
    {
        auto* rb2 = entity->TryGetBehavior<RigidBody>();
        if (rb2 != nullptr && rb2 == this)
            continue;
        if (rb2 == nullptr)
            continue;
        auto* m = entity->TryGetBehavior<CollisionShape>();
        if (m == nullptr)
            continue;

        Colliding c;
        if (cdType == CollisionDetection::DISCRETE)
            c = checkDiscreteCollision(newPosition, *m);
        else if (cdType == CollisionDetection::CONTINUOUS)
            c = checkContinuousCollision(newPosition, *m);

        if (c.collision) {
            auto rb = this;
            // Calculate relative velocity
            Vector2 rv = rb2->velocity - rb->velocity;
            // Calculate relative velocity in terms of the normal direction
            float velAlongNormal = rv.dot(c.normal);

            // Do not resolve if velocities are separating
            if(velAlongNormal > 0)
                continue;

            // Calculate restitution
            float e = 0.5f;

            float inv_mass1;
            if (rb->mass == 0)
                inv_mass1 = 0;
            else
                inv_mass1 = 1 / rb->mass;
            float inv_mass2;
            if (rb2->mass == 0)
                inv_mass2 = 0;
            else
                inv_mass2 = 1 / rb2->mass;

            // Calculate impulse scalar
            float j = -(1 + e) * velAlongNormal;
            j /= inv_mass1 + inv_mass2;
            Vector2 impulse = j * c.normal;

            float ratio1 = inv_mass1;
            rb->velocity -= ratio1 * impulse;

            auto ratio2 = inv_mass2;
            rb2->velocity += ratio2 * impulse;

            //Fixing velocities so objects don't look buggy
            if (abs(rb->velocity.magnitude()) < 0.8)
            {
                rb->velocity.x *= abs(c.normal.y);
                rb->velocity.y *= abs(c.normal.x);
            }

            if (abs(rb2->velocity.magnitude()) < 0.8)
            {
                rb2->velocity.x *= abs(c.normal.y);
                rb2->velocity.y *= abs(c.normal.x);
            }
        }
    }
}

#pragma region "Collision detection"

struct Colliding RigidBody::checkContinuousCollision(const Vector2 startPos, CollisionShape& m) {
    auto otherPos = (Vector2)m.getBounds()->getPosition();
    auto deltaTime = GlobalVars::fixedDeltaTime;
    auto finalPos = startPos + (oldVelocity * deltaTime + (0.5f * this->velocity * deltaTime) * 1000.f);
    if ((startPos.x > otherPos.x && finalPos.x < otherPos.x || velocity.x == 0) &&
        (startPos.y > otherPos.y && finalPos.y < otherPos.y || velocity.y == 0))
        return Colliding{false};

    //std::cout << entity->id << " " << m.entity->id << " yes" << std::endl;

    const float precision = 0.01f;
    const int max = (int)(1 / precision);

    auto direction = finalPos - startPos;
    auto sprite = parent;

    Colliding colliding{false};
    for (int i = 0; i <= max; i++) {
        float j = (float)i / (float)max;

        //auto pos = startPos + (distance * j * deltaTime * 1000.f);
        auto pos = startPos + j * direction;

        colliding = checkDiscreteCollision(pos, m);
        if (colliding.collision)
        {
            sprite->setPosition((sf::Vector2f)pos);
            break;
        }
    }

    //  std::cout << entity->id << " " << colliding.collision << std::endl;

    return colliding;
}

Colliding RigidBody::checkDiscreteCollision(Vector2 endPos, CollisionShape& m)
{
    collisionShape->getBounds()->setPosition((sf::Vector2f) endPos);
    return collisionShape->satCollision(m);
}

#pragma endregion