#ifndef PHYSICSSIMULATIONS_ROPETEST_H
#define PHYSICSSIMULATIONS_ROPETEST_H

#include "Simulations.h"

using namespace Simulations;

class RopeTestSimulation : public Simulation {
private:
    Rope rope = Rope(Vector2(350, 250), Vector2(490, 250));
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
        graphsManager.addPoint(0, time, ropeRb.velocity.x);
        graphsManager.addPoint(1, time, ropeRb.velocity.y);
    }
};

#endif //PHYSICSSIMULATIONS_ROPETEST_H
