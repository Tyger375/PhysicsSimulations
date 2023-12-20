#ifndef PHYSICSSIMULATIONS_ROPERIGIDBODY_H
#define PHYSICSSIMULATIONS_ROPERIGIDBODY_H

#include "Rope.h"
#include "../Behaviors/RigidBody/RigidBody.h"
#include "RopeMember.h"

class RopeRigidBody : public RigidBody
{
private:
    Rope* rope;
    RopeMember* last{};
    CircleShape* lastCollision{};
    float angle = 0.f;
    float angularVel = 0.f;
    //Vector2 acceleration = Vector2(10.f, 10.f);
    std::vector<struct Debug::Line> debugs;
public:
    RopeRigidBody(
            Rope* rope,
            CollisionDetection collisionDetectionType,
            bool useGravity = true
            ) :
            last(&*rope->members.end().operator--()), // last member
            lastCollision(new CircleShape(
                    last->pointer(),
                    1
            )), // last collision member
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
        this->velocity = Vector2();
    }
    void update() override
    {
        auto deltaTime = GlobalVars::deltaTime;
        auto r = rope->distance().normalize();
        auto direction = Vector2{-r.y, r.x};
        //std::cout << direction << std::endl;
        /*if (std::abs(rope->getTension() - 1) < 0.1)
        {
            auto v = direction * velocity.magnitude();
            velocity = v;
        }*/
        velocity = direction * 5.f;

        auto acceleration = direction * 0.f;

        auto radius = rope->length.magnitude();
        auto vel = oldVelocity.magnitude();
        auto centripetal = (vel * vel) / radius;
        auto Ac = r * -1 * centripetal * deltaTime;
        std::cout << Ac << " " << Ac.magnitude() << std::endl;
        velocity = oldVelocity + (acceleration + Ac) * deltaTime;
        //velocity += Ac;

        struct Debug::Line line;
        line.start = Vector2(150.f, 150.f);
        line.direction = r * -1;
        line.distance = centripetal * 100.f;
        debugs.push_back(line);

        //velocity = direction * velocity.magnitude();

        auto pos = Vector2(parent->getPosition());
        //s(t) = s0 + v0 * t + 0.5 * a * t^2
        auto m = 0.5f * velocity * deltaTime;
        //std::cout << m << std::endl;
        //Transforming meters to pixels
        //1 m = 1000 px
        //pos:1 = x: 1000
        m *= 1000.f;
        pos += m;
        //RigidBody::update();

        rope->setEnd(pos);

        oldVelocity = velocity;
    }

    void debug(sf::RenderWindow* window) {
        for (auto member : debugs)
        {
            //std::cout << "Test" << std::endl;
            Debug::drawLine(window, member);
        }
        debugs.clear();
    }
};

#endif //PHYSICSSIMULATIONS_ROPERIGIDBODY_H
