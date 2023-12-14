#ifndef PHYSICSSIMULATIONS_RIGIDBODY_H
#define PHYSICSSIMULATIONS_RIGIDBODY_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../main.h"
#include "../CollisionShape/CollisionShape.h"

namespace PhysicsLaws
{
    const float GravityAcceleration = -9.81f; // m/s^2
}

enum CollisionDetection
{
    DISCRETE,
    CONTINUOUS
};

enum ForceMode
{
    ACCELERATION,
    FORCE,
    IMPULSE,
    VELOCITY
};

class RigidBody : public Behavior {
protected:
    sf::Shape* parent;
    CollisionShape* collisionShape;
    Vector2 oldVelocity{};

    CollisionDetection cdType;
    float mass;
    const float Fc = 0.1; //Friction coefficient
public:
    Vector2 velocity{};
    bool useGravity;

    RigidBody(
            sf::Shape* parent,
            CollisionShape* bounds,
            CollisionDetection collisionDetectionType,
            float mass,
            bool useGravity = true
            )
    {
        this->parent = parent;
        this->collisionShape = bounds;
        this->cdType = collisionDetectionType;
        this->mass = mass;
        this->useGravity = useGravity;
    }

    void update() override;

    void addForce(Vector2 force, ForceMode mode=FORCE)
    {
        Vector2 v;
        if (mode == FORCE)
        {
            //Force => v is Kg*m/s^2
            v = (force * GlobalVars::deltaTime) / mass;
        }
        else if (mode == ACCELERATION)
        {
            //Acceleration => v is m/s^2
            v = force * GlobalVars::deltaTime;
        }
        else if (mode == IMPULSE)
        {
            //Momentum change => v is Kg*m/s
            v = force / mass;
        }
        else if (mode == VELOCITY)
            v = force;

        this->velocity += v;
    }

    //Collision detection
    virtual bool checkDiscreteCollision(Vector2, CollisionShape&);
    virtual bool checkContinuousCollision(Vector2, CollisionShape&, Vector2);
    bool isOnGround(Vector2, Vector2, Vector2, CollisionShape**);
};


#endif //PHYSICSSIMULATIONS_RIGIDBODY_H
