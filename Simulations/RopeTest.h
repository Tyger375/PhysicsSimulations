#ifndef PHYSICSSIMULATIONS_ROPETEST_H
#define PHYSICSSIMULATIONS_ROPETEST_H

#include "Simulations.h"

using namespace Simulations;

class RopeTestSimulation : public Simulation {
private:
    Rope rope = Rope(Vector2(150, 50), Vector2(290, 50));
    RopeRigidBody ropeRb = RopeRigidBody(&rope, CONTINUOUS, false);
public:
    void onCreate() override {
        ropeRb.velocity = Vector2(5.f, 0.f);
        rope.AddBehavior(&ropeRb);
        GlobalVars::entities.push_back(&rope);
    }
    void onEvent(sf::Event e) override {

    }
    void onUpdate() override {

    }
    void onDrawGraphs() override {
        auto vel = ropeRb.velocity.magnitude();
        auto pos = rope.members.end().operator--()->getSprite().getPosition();
        graphsManager.addPoint(0, time, vel);
        graphsManager.addPoint(1, time, pos.y);
    }
};

#endif //PHYSICSSIMULATIONS_ROPETEST_H
