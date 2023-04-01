#ifndef PHYSICSSIMULATIONS_RIGIDBODY_H
#define PHYSICSSIMULATIONS_RIGIDBODY_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../main.h"
#include "../CollisionShape/CollisionShape.h"

namespace PhysicsLaws
{
    const float GravityAcceleration = -9.81f; // Newton (kg * m)/s^2
}

enum CollisionDetection
{
    DISCRETE,
    CONTINUOUS
};

class RigidBody {
private:
    sf::RectangleShape* parent;
    CollisionShape* collisionShape;
    sf::Vector2f velocity{};

    CollisionDetection cdType;
    float mass;
public:
    RigidBody(sf::RectangleShape* parent, CollisionShape* bounds, CollisionDetection collisionDetectionType, float mass)
    {
        this->parent = parent;
        this->collisionShape = bounds;
        this->cdType = collisionDetectionType;
        this->mass = mass;
    }

    void update();

    //Collision detection
    bool checkDiscreteCollision(sf::Vector2f, const sf::RectangleShape&);
    bool checkContinuousCollision(sf::Vector2f, const sf::RectangleShape&, sf::Vector2f);
    bool isOnGround(sf::Vector2f, sf::Vector2f, sf::Vector2f);
};


#endif //PHYSICSSIMULATIONS_RIGIDBODY_H
