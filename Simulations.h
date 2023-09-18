#include "Vector2/Vector2.h"
#include "Object.h"
#include "Rope/RopeRigidBody.h"

namespace Simulations {
    void Test() {
        auto size = Vector2(50, 50);
        static Object obj(size, size);
        obj.setPosition(Vector2(size.x/2, size.y/2) + Vector2(40, -0));

        auto groundSize = Vector2(500, 50);
        static Object ground(groundSize, groundSize, sf::Color::Green);
        ground.rb.useGravity = false;
        ground.setRotation(20);
        ground.setPosition(Vector2(-100, 300));

        auto ground2Size = Vector2(500 * 10, 50);
        static Object ground2(ground2Size, ground2Size, sf::Color::Blue);
        ground2.rb.useGravity = false;
        ground2.setPosition(Vector2(250, 450));

        GlobalVars::entities.push_back(&obj);
        GlobalVars::entities.push_back(&ground);
        GlobalVars::entities.push_back(&ground2);
    }

    void RopeTest() {
        static Rope rope(Vector2(150, 50), Vector2(290, 50));
        static RopeRigidBody ropeRb(&rope, CONTINUOUS, false);
        ropeRb.addForce(Vector2(0, 1), VELOCITY);
        rope.AddBehavior(&ropeRb);
        GlobalVars::entities.push_back(&rope);
    }

    void Projectile() {
        auto size = Vector2(50, 50);
        static Object obj(size, size);
        obj.setPosition(Vector2(50, 400));

        auto ground2Size = Vector2(500 * 10, 50);
        static Object ground(ground2Size, ground2Size, sf::Color::Blue);
        ground.rb.useGravity = false;
        ground.setPosition(Vector2(250, 450));

        GlobalVars::entities.push_back(&obj);
        GlobalVars::entities.push_back(&ground);
    }
}