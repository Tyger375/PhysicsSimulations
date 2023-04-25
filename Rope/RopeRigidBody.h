#ifndef PHYSICSSIMULATIONS_ROPERIGIDBODY_H
#define PHYSICSSIMULATIONS_ROPERIGIDBODY_H

#include "../Behaviors/RigidBody/RigidBody.h"
#include "Rope.h"

class RopeRigidBody : public RigidBody
{
private:
    Rope* rope;
    RopeMember* last{};
    CircleShape* lastCollision{};
public:
    Vector2 lastPos{};
    RopeRigidBody(
            Rope* rope,
            CollisionDetection collisionDetectionType,
            bool useGravity = true
            ) :
            last(rope->members.end().operator--().base()),
            lastCollision(new CircleShape(
                    last->pointer(),
                    1
            )),
            RigidBody(
                    nullptr,
                    nullptr,
                    collisionDetectionType,
                    1,
                    useGravity
            )
    {
        this->parent = last->pointer();
        this->collisionShape = lastCollision;
        this->rope = rope;
    }
    void update() override
    {
        if (std::abs(rope->getTension() - 1) < 0.1)
        {
            auto r = rope->distance();
            auto v = Vector2{-r.y, r.x}.normalize() * velocity.magnitude();
            velocity = v;
        }

        RigidBody::update();

        auto pos = parent->getPosition();
        rope->setEnd(pos);
    }
};

#endif //PHYSICSSIMULATIONS_ROPERIGIDBODY_H
