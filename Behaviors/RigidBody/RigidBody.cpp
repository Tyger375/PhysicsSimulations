#include "RigidBody.h"
#include "../CollisionShape/Detection.cuh"

void RigidBody::update() {
    auto deltaTime = GlobalVars::fixedDeltaTime;

    auto pos = Vector2(parent->getPosition());
    //s(t) = s0 + v0 * t + 0.5 * a * t^2
    auto m = this->velocity * deltaTime;

    //Transforming meters to pixels
    //1 m = 100 px
    //pos:1 = x: 100
    m *= 100.f;
    pos += m;

    parent->setPosition((sf::Vector2f) pos);

    auto oldRot = parent->getRotation();
    auto rot = math::degToRad(oldRot);

    auto r = this->angularVelocity;
    rot += r;

    auto deg = (float)math::radToDeg(rot);
    auto deltaR = std::abs(deg - oldRot);
    if (deltaR < 1.5f)
        deg = std::round(deg / 10.f) * 10.f;
    parent->setRotation(deg);
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
        float staticF = (rb->staticFriction + rb2->staticFriction) / 2.f;
        float dynamicF = (rb->dynamicFriction + rb2->dynamicFriction) / 2.f;

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

        // calculating impulses
        for (int i = 0; i < contactCount; i++) {
            auto contact = contacts[i];
            auto dA = contact - thisPos;
            auto dB = contact - otherPos;

            auto orthogonalA = dA.orthogonal();
            auto orthogonalB = dB.orthogonal();

            directionsA[i] = dA;
            directionsB[i] = dB;

            auto angularLinearVelocityA = orthogonalA * rb->angularVelocity;
            auto angularLinearVelocityB = orthogonalB * rb2->angularVelocity;

            // Calculate relative velocity
            Vector2 rv = (rb2->velocity + angularLinearVelocityB) - (rb->velocity + angularLinearVelocityA);
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

        // applying impulses
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

        // calculating friction
        // calculating impulses
        for (int i = 0; i < contactCount; i++) {
            auto dA = directionsA[i];
            auto dB = directionsB[i];

            auto orthogonalA = dA.orthogonal();
            auto orthogonalB = dB.orthogonal();

            auto angularLinearVelocityA = orthogonalA * rb->angularVelocity;
            auto angularLinearVelocityB = orthogonalB * rb2->angularVelocity;

            // Calculate relative velocity
            Vector2 rv = (rb2->velocity + angularLinearVelocityB) - (rb->velocity + angularLinearVelocityA);

            Vector2 tang = rv - rv.dot(normal) * normal;

            if (tang == Vector2())
                continue;

            tang = tang.normalize();

            // orthogonal dot normal
            auto oNa = orthogonalA.dot(tang);
            auto oNb = orthogonalB.dot(tang);

            auto denom = (inv_mass1 + inv_mass2) + ((oNa * oNa) * inv_inertia1) + ((oNb * oNb) * inv_inertia2);
            // Calculate impulse scalar
            double jFriction = -rv.dot(tang);
            jFriction /= denom;
            //Applying impulse as half for when we have 2 contact points
            jFriction /= (float)contactCount;

            Vector2 friction;
            auto j = impulses[i].magnitude();
            if (std::abs(jFriction) <= j * staticF)
                friction = jFriction * tang;
            else
                friction = -j * tang * dynamicF;
            impulses[i] = friction;
        }

        // applying impulses
        for (int i = 0; i < contactCount; i++) {
            auto friction = impulses[i];
            auto dA = directionsA[i];
            auto dB = directionsB[i];

            // Friction force has to be less or equal to Fn * Static friction

            rb->velocity -= friction * inv_mass1;
            rb->angularVelocity -= dA.cross(friction).z * inv_inertia1;

            rb2->velocity += friction * inv_mass2;
            rb2->angularVelocity += dB.cross(friction).z * inv_inertia2;
        }

        free(impulses);
        free(directionsA);
        free(directionsB);
    }
}

#pragma region "Collision detection"

struct Colliding RigidBody::checkContinuousCollision(const Vector2 startPos, CollisionShape& m) {
    auto otherPos = (Vector2)m.getBounds()->getPosition();
    auto deltaTime = GlobalVars::fixedDeltaTime;
    auto v = this->velocity * deltaTime * 100.0;
    auto finalPos = startPos + v;

    auto size = m.getSize();
    if (m.getType() == ShapeType::CIRCLE)
        size.y = size.x;
    auto bounds = math::getBounds(size, startPos, v);

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