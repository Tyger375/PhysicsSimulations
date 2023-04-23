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
    IMPULSE
};

class RigidBody {
private:
    sf::Shape* parent;
    CollisionShape* collisionShape;
    Vector2 velocity{};

    CollisionDetection cdType;
    float mass;
    const float Fc = 0.1; //Friction coefficient
public:
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
        this->useGravity = true;
    }

    void update();

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

        this->velocity += v;
    }

    //Collision detection
    bool checkDiscreteCollision(Vector2, CollisionShape&);
    bool checkContinuousCollision(Vector2, CollisionShape&, Vector2);
    bool isOnGround(Vector2, Vector2, Vector2, CollisionShape**);
};


#endif //PHYSICSSIMULATIONS_RIGIDBODY_H
