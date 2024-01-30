#ifndef PHYSICSSIMULATIONS_ROPERIGIDBODY_H
#define PHYSICSSIMULATIONS_ROPERIGIDBODY_H

#include "Rope.h"
#include "RopeMember.h"
#include "../Behaviors/RigidBody/RigidBody.h"
#include "../Behaviors/CircleShape/CircleShape.h"

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
    Vector2 acceleration{};
    Vector2 oldVelocity{};
    Vector2 test{};
    explicit RopeRigidBody(
            Rope* rope
    ) :
            last(&*rope->members.end().operator--()), // last member
            lastCollision(new CircleShape(
                    last->pointer(),
                    last,
                    1
            )), // last collision member
            Behavior(rope)
    {
        this->rope = rope;
        this->velocity = Vector2(0, 1);
        this->test = velocity;
    }

    void update() override
    {
        auto deltaTime = GlobalVars::fixedDeltaTime;
        auto r = rope->distance().normalize();
        auto direction = Vector2{-r.y, r.x};
        //std::cout << direction << std::endl;
        /*if (std::abs(rope->getTension() - 1) < 0.1)
        {
            auto v = direction * velocity.magnitude();
            velocity = v;
        }*/
        //velocity = direction * .5f;

        //auto acceleration = (direction * 100.f);
        /*
         * v
         */
        //acceleration = direction.normalize() * 1;

        //test += acceleration;
        //velocity += acceleration * deltaTime;

        //velocity = test.magnitude() * velocity.normalize() * deltaTime;
        test += Vector2(0, .1);
        velocity = test.magnitude() * direction;

        auto radius = (double) rope->length.magnitude();
        auto vel = (double) velocity.magnitude();
        auto centripetal = pow(vel, 2) / radius;
        std::cout << vel << " " << velocity.magnitude() << " " << vel * vel << " " << radius << " " << centripetal << std::endl;
        auto Ac = r * -1 * (float) centripetal;

        velocity += Ac;

        //test += Ac;

        //velocity += (acceleration + Ac) * deltaTime;

        //std::cout << Ac << " " << Ac.magnitude() << std::endl;
        //velocity = acceleration * deltaTime;
        //velocity += Ac * deltaTime;
        //std::cout << velocity.dot(direction) << " " << Ac << " " << vel << " " << centripetal << std::endl;
        //velocity += Ac;

        //struct Debug::Line line;
        //line.start = Vector2(150.f, 150.f);
        //line.start = (Vector2)last->getSprite().getPosition();
        //line.direction = r * -1;
        //line.direction = velocity.normalize();
        struct Debug::Line line;
        line.color = sf::Color::Yellow;
        line.start = (Vector2)last->getSprite().getPosition();
        line.direction = Ac.normalize();
        line.distance = Ac.magnitude() * 100;

        struct Debug::Line line2;
        line2.color = sf::Color::Green;
        line2.start = (Vector2)last->getSprite().getPosition();
        line2.direction = r * -1;
        line2.distance = 100;

        struct Debug::Line line3;
        line3.color = sf::Color::Blue;
        line3.start = (Vector2)last->getSprite().getPosition();
        line3.direction = velocity.normalize();
        line3.distance = velocity.magnitude() * 1000;

        debugs.clear();
        debugs.push_back(line);
        debugs.push_back(line2);
        debugs.push_back(line3);

        //velocity = direction * velocity.magnitude();

        auto pos = Vector2(last->getSprite().getPosition());
        //s(t) = s0 + v0 * t + 0.5 * a * t^2
        //auto m = oldVelocity * deltaTime + 0.5f * velocity * deltaTime;
        auto m = 0.5 * velocity * deltaTime;
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
    }
};

#endif //PHYSICSSIMULATIONS_ROPERIGIDBODY_H
