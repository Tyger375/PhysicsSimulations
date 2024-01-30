#ifndef PHYSICSSIMULATIONS_ROPETEST_H
#define PHYSICSSIMULATIONS_ROPETEST_H

#include "Simulations.h"
#include "../Rope/Rope.h"
#include "../Rope/RopeRigidBody.h"

using namespace Simulations;

class RopeTestSimulation : public Simulation {
private:
    Rope rope = Rope(Vector2(450, 350), Vector2(590, 350));
    RopeRigidBody ropeRb = RopeRigidBody(&rope);
public:
    void onCreate() override {
        graphsManager.addGraph("Graph");
        graphsManager.addGraph("Graph");
        graphsManager.addGraph("Graph");
        graphsManager.build();
        //ropeRb.velocity = Vector2(5.f, 0.f);
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
        graphsManager.addPoint(2, time, ((Vector2)pos).magnitude());
    }
};

#endif //PHYSICSSIMULATIONS_ROPETEST_H
