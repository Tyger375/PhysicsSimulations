#include "RigidBody.h"
#include "../CollisionShape/Detection.cuh"

void RigidBody::update() {
    auto deltaTime = GlobalVars::fixedDeltaTime;

    auto pos = Vector2(parent->getPosition());
    //s(t) = s0 + v0 * t + 0.5 * a * t^2
    auto m = this->velocity * deltaTime;

    //Transforming meters to pixels
    //1 m = 1000 px
    //pos:1 = x: 1000
    m *= 1000.f;
    pos += m;

    auto rot = math::degToRad(parent->getRotation());

    auto r = this->angularVelocity;
    rot += r;

    parent->setPosition((sf::Vector2f) pos);
    parent->setRotation((float)math::radToDeg(rot));


    /*
     * TODO: Reimplement inclined plane and friction
    if (!isOnGround((Vectors)parent->getPosition(), pos, 0.5f * velocity + oldVelocity, &obj))
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
                auto acceleration = Vectors(
                        ParallelAcceleration * std::sin(wAlpha),
                        ParallelAcceleration * std::cos(wAlpha)
                );
                auto frictionAcceleration = Fa / mass;
                auto fAcceleration = Vectors(
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

        if (!c.collision)
            continue;

        this->debugPoints.push_back({c.collidingPoints.pointA, sf::Color::Yellow});
        this->debugPoints.push_back({c.collidingPoints.pointB, sf::Color::Yellow});

        this->debugLines.push_back({(Vector2)this->parent->getPosition(), c.normal, 30.f, sf::Color::Green});

        auto rb = this;

        // Calculate restitution
        float e = std::min(rb->restitution, rb2->restitution);

        Vector2 contacts[] = {c.collidingPoints.pointA, c.collidingPoints.pointB};

        auto thisPos = (Vector2)rb->parent->getPosition();
        auto otherPos = (Vector2)rb2->parent->getPosition();

        auto contactCount = c.collidingPoints.contactCount;

        auto normal = c.normal;

        auto* impulses = new Vector2[2] { Vector2(), Vector2() };
        auto* directionsA = new Vector2[2] { Vector2(), Vector2() };
        auto* directionsB = new Vector2[2] { Vector2(), Vector2() };

        //
        auto inv_mass1 = rb->inv_mass;
        auto inv_mass2 = rb2->inv_mass;

        auto inv_inertia1 = rb->inv_inertia;
        auto inv_inertia2 = rb2->inv_inertia;

        for (int i = 0; i < contactCount; i++) {
            auto contact = contacts[i];
            auto dA = contact - thisPos;
            auto dB = contact - otherPos;

            auto orthogonalA = dA.orthogonal();
            auto orthogonalB = dB.orthogonal();

            directionsA[i] = dA;
            directionsB[i] = dB;

            auto angularLinearVelocityA = orthogonalA * rb->oldAngular;
            auto angularLinearVelocityB = orthogonalB * rb2->oldAngular;

            // Calculate relative velocity
            Vector2 rv = (rb2->oldVelocity + angularLinearVelocityB) - (rb->oldVelocity + angularLinearVelocityA);
            // Calculate relative velocity in terms of the normal direction
            double velAlongNormal = rv.dot(normal);

            // Do not resolve if velocities are separating
            if(velAlongNormal > 0)
                continue;

            // orthogonal dot normal
            auto oNa = orthogonalA.dot(normal);
            auto oNb = orthogonalB.dot(normal);

            auto denom = (inv_mass1 + inv_mass2) + ((oNa * oNa) * inv_inertia1) + ((oNb * oNb) * inv_inertia2);
            // Calculate impulse scalar
            double j = -(1. + e) * velAlongNormal;
            j /= denom;
            //Applying impulse as half for when we have 2 contact points
            j /= (float)contactCount;

            Vector2 impulse = j * normal;
            impulses[i] = impulse;
        }

        for (int i = 0; i < contactCount; i++) {
            auto impulse = impulses[i];
            auto dA = directionsA[i];
            auto dB = directionsB[i];

            rb->velocity -= impulse * inv_mass1;
            rb->angularVelocity -= dA.cross(impulse).z * inv_inertia1;

            rb2->velocity += impulse * inv_mass2;
            rb2->angularVelocity += dB.cross(impulse).z * inv_inertia2;
        }

        const double percent = 0.2; // usually 20% to 80%

        const double slop = 0.01; // usually 0.01 to 0.1

        Vector2 correction = std::max(c.penetration - slop, 0. ) / (inv_mass1 + inv_mass2) * percent * c.normal;

        rb->parent->setPosition(rb->parent->getPosition() - (sf::Vector2f)(inv_mass1 * correction));
        rb2->parent->setPosition(rb2->parent->getPosition() + (sf::Vector2f)(inv_mass2 * correction));


        //Fixing velocities so objects don't look buggy
        /*if ((rb->velocity.magnitude()) < 0.1)
        {
            rb->velocity.x *= abs(c.normal.y);
            rb->velocity.y *= abs(c.normal.x);
        }

        if ((rb2->velocity.magnitude()) < 0.1)
        {
            rb2->velocity.x *= abs(c.normal.y);
            rb2->velocity.y *= abs(c.normal.x);
        }*/

        if ((rb->angularVelocity) < 0.001)
        {
            rb->angularVelocity = 0;
        }

        if ((rb2->angularVelocity) < 0.001)
        {
            rb2->angularVelocity = 0;
        }

        delete[] impulses;
        delete[] directionsA;
        delete[] directionsB;
    }
}

#pragma region "Collision detection"

struct Colliding RigidBody::checkContinuousCollision(const Vector2 startPos, CollisionShape& m) {
    auto otherPos = (Vector2)m.getBounds()->getPosition();
    auto deltaTime = GlobalVars::fixedDeltaTime;
    auto finalPos = startPos + (this->velocity * deltaTime * 1000.0);

    auto size = m.getSize();
    if (m.getType() == ShapeType::CIRCLE)
        size.y = size.x;
    auto bounds = math::getBounds(size, startPos, this->velocity * deltaTime * 1000.0);

    auto first = bounds.first;
    auto second = bounds.second;

    if ((first.x > otherPos.x && second.x < otherPos.x) &&
        (first.y > otherPos.y && second.y < otherPos.y))
        return Colliding{false};

    auto sprite = parent;
    auto direction = finalPos - startPos;
    Colliding colliding;

#ifdef USECUDA
    //STILL IN BETA
    colliding = checkContinuousWrapper(startPos, direction, sprite, m.getBounds());
#else
    const float precision = 0.1f;
    const int max = (int)(1 / precision);

    auto pPos = startPos;

    for (int i = 0; i <= max; i++) {
        float j = (float)i / (float)max;

        auto pos = startPos + j * direction;

        colliding = checkDiscreteCollision(pos, m);
        if (colliding.collision)
        {
            sprite->setPosition((sf::Vector2f) pPos);
            break;
        }
        pPos = pos;
    }
#endif

    return colliding;
}

Colliding RigidBody::checkDiscreteCollision(Vector2 endPos, CollisionShape& m)
{
    collisionShape->getBounds()->setPosition((sf::Vector2f) endPos);
    return collisionShape->satCollision(m);
}

#pragma endregion