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

    CollisionDetection cdType;
    const float Fc = 0.1; //Friction coefficient
public:
    double mass{};
    double inv_mass{};

    double inertia{};
    double inv_inertia{};

    float restitution = .0;

    Vector2 velocity{};
    double angularVelocity = 0.f;
    bool useGravity;
    std::vector<struct Debug::Line> debugLines;
    std::vector<struct Debug::Point> debugPoints;

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

        setDensity(10);

        this->useGravity = useGravity;
    }

    void setDensity(double newDensity) {
        //Mass and inverse mass
        auto shapeType = collisionShape->getType();
        if (shapeType == ShapeType::CIRCLE) {
            auto radius = (float)collisionShape->getSize().magnitude();
            this->mass = newDensity * M_PI * radius * radius;
        }
        else {
            auto size = collisionShape->getSize();
            auto width = size.x;// * 10.f;
            auto height = size.y;// * 10.f;
            this->mass = newDensity * width * height;
        }

        if (mass == 0.f)
            this->inv_mass = 0;
        else
            this->inv_mass = 1. / mass;

        //Inertia and inverse inertia

        if (shapeType == ShapeType::CIRCLE) {
            auto radius = (float)collisionShape->getSize().magnitude();
            inertia = 1.f / 2.f * mass * pow(radius, 2.f);
        }
        else if (shapeType == ShapeType::RECTANGLE) {
            auto size = collisionShape->getSize();
            auto width = size.x;// * 10.f;
            auto height = size.y;// * 10.f;
            //inertia = (1. / 12.) * mass * (length * length + height * height);
            inertia = (mass / 12.) * (width * width + height * height);
        }

        if (inertia == 0)
            inv_inertia = 0;
        else
            inv_inertia = 1. / inertia;
    }

    void checkCollisions(Vector2);

    void updateVars() {
        auto deltaTime = GlobalVars::fixedDeltaTime;
        if (useGravity)
        {
            //Adding gravity
            this->velocity -= .5f * Vector2(0, PhysicsLaws::GravityAcceleration) * deltaTime;
        }
    }

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
    virtual Colliding checkContinuousCollision(Vector2, CollisionShape&);

    void debug(sf::RenderWindow* window) {
        for (auto member : debugPoints)
        {
            //std::cout << "Test" << std::endl;
            Debug::drawPoint(window, member);
        }
        for (auto member : debugLines)
        {
            Debug::drawLine(window, member);
        }

        if (!debugLines.empty())
        {
            std::cout << "";
        }
        debugLines.clear();
        debugPoints.clear();
    }
};


#endif //PHYSICSSIMULATIONS_RIGIDBODY_H
