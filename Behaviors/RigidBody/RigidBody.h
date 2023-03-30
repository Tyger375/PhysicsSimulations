#ifndef PHYSICSSIMULATIONS_RIGIDBODY_H
#define PHYSICSSIMULATIONS_RIGIDBODY_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../main.h"

namespace PhysicsLaws
{
    const float GravityAcceleration = -9.81f; // Newton (kg * m)/s^2
}

class RigidBody {
private:
    sf::RectangleShape* parent;
    sf::RectangleShape* parentBounds;
    sf::Vector2f velocity{};

    float Tg = 0.f;
public:
    RigidBody(sf::RectangleShape* parent, sf::RectangleShape* bounds)
    {
        this->parent = parent;
        this->parentBounds = bounds;
    }

    bool isColliding()
    {
        bool colliding = false;
        auto bounds = this->parentBounds->getGlobalBounds();
        for (int i = 0; i < GlobalVars::size; i++)
        {
            auto m = GlobalVars::grounds[i];
            colliding = bounds.intersects(m.getGlobalBounds());
            if (colliding)
                break;
        }

        return colliding;
    }

    void update() {
        auto deltaTime = GlobalVars::deltaTime;
        auto oldVelocity = this->velocity;

        bool colliding = isColliding();

        //std::cout << deltaTime << std::endl;

        //Adding gravity
        if (colliding)
            this->velocity = sf::Vector2f(0, 0);
        else
            this->velocity -= sf::Vector2f(0, 0.5f * PhysicsLaws::GravityAcceleration * deltaTime);

        auto pos = parent->getPosition();
        auto m = oldVelocity * deltaTime + this->velocity * deltaTime;
        //pos:1 = x: 1000
        m = (m * 1000.f) / 1.f;
        pos += m;
        //std::cout << pos.y << std::endl;
        auto oldPos = this->parentBounds->getPosition();
        this->parentBounds->setPosition(pos);
        if (!isColliding())
            parent->setPosition(pos);
        else
            velocity = sf::Vector2f();

    }
};


#endif //PHYSICSSIMULATIONS_RIGIDBODY_H
