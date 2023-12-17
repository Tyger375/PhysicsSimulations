#ifndef PHYSICSSIMULATIONS_RIGIDBODY_H
#define PHYSICSSIMULATIONS_RIGIDBODY_H

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
    const float Fc = 0.1; //Friction coefficient
public:
    float mass;
    Vector2 velocity{};
    bool useGravity;

    RigidBody(
            sf::Shape* parent,
            Entity* entityParent,
            CollisionShape* bounds,
            CollisionDetection collisionDetectionType,
            float mass,
            bool useGravity = true
            ) : Behavior(entityParent)
    {
        this->parent = parent;
        this->collisionShape = bounds;
        this->cdType = collisionDetectionType;
        this->mass = mass;
        this->useGravity = useGravity;
    }

    void checkCollisions(Vector2, Vector2);

    void update() override;

    void addForce(Vector2 force, ForceMode mode=FORCE)
    {
        if (mode == FORCE)
        {
            //Force => v is Kg*m/s^2
            this->velocity += (force * GlobalVars::deltaTime) / mass;
        }
        else if (mode == ACCELERATION)
        {
            //Acceleration => v is m/s^2
            this->velocity += force * GlobalVars::deltaTime;
        }
        else if (mode == IMPULSE)
        {
            //Momentum change => v is Kg*m/s
            this->velocity += force / mass;
        }
        else if (mode == VELOCITY)
            this->velocity += force;
    }

    //Collision detection
    virtual Colliding checkDiscreteCollision(Vector2, CollisionShape&);
    virtual Colliding checkContinuousCollision(Vector2, CollisionShape&, Vector2);

    [[maybe_unused]] bool isOnGround(Vector2, Vector2, Vector2, CollisionShape**);
};


#endif //PHYSICSSIMULATIONS_RIGIDBODY_H
