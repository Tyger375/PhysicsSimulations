#ifndef PHYSICSSIMULATIONS_ROPERIGIDBODY_H
#define PHYSICSSIMULATIONS_ROPERIGIDBODY_H

#include "Rope.h"
#include "RopeMember.h"
#include "../Behaviors/RigidBody/RigidBody.h"

class RopeRigidBody : public Behavior
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
    Vector2 velocity{};
    Vector2 oldVelocity{};
    RopeRigidBody(
            Rope* rope,
            CollisionDetection collisionDetectionType,
            bool useGravity = true
    ) :
            last(&*rope->members.end().operator--()), // last member
            lastCollision(new CircleShape(
                    last->pointer(),
                    1
            )) // last collision member
    {
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
        //velocity = direction * 5.f;

        //auto acceleration = (direction * 100.f);
        auto acceleration = direction * 9.81;

        auto radius = rope->length.magnitude();
        auto vel = velocity.magnitude();
        auto centripetal = (vel * vel) / radius;
        auto Ac = r * -1 * centripetal;
        //std::cout << Ac << " " << Ac.magnitude() << std::endl;
        velocity = (acceleration + Ac) * deltaTime;
        std::cout << velocity.dot(direction) << std::endl;
        //velocity += Ac;

        //struct Debug::Line line;
        //line.start = Vector2(150.f, 150.f);
        //line.start = (Vector2)last->getSprite().getPosition();
        //line.direction = r * -1;
        //line.direction = velocity.normalize();
        struct Debug::Line line;
        line.start = (Vector2)last->getSprite().getPosition();
        line.direction = Ac.normalize();
        line.distance = 100.f;

        struct Debug::Line line2;
        line2.start = (Vector2)last->getSprite().getPosition();
        line2.direction = acceleration.normalize();
        line2.distance = 100.f;

        debugs.push_back(line);
        debugs.push_back(line2);

        //velocity = direction * velocity.magnitude();

        auto pos = Vector2(last->getSprite().getPosition());
        //s(t) = s0 + v0 * t + 0.5 * a * t^2
        auto m = oldVelocity * deltaTime + 0.5f * velocity * deltaTime;
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
