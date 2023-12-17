#ifndef PHYSICSSIMULATIONS_PROJECTILE_H
#define PHYSICSSIMULATIONS_PROJECTILE_H

#include "Simulations.h"
#include "Vector2/Vector2.h"
#include "Object/Object.h"
#include "../Utils.h"

using namespace Simulations;

class ProjectileSimulation : public Simulation {
private:
    Vector2 size = Vector2(50, 50);
    Object player = Object(size, size);
public:
    void onCreate() override {
        player.rb.useGravity = true;
        player.rb.mass = 10.f;
        player.setPosition(Vector2(50, 600));

        auto ground2Size = Vector2(500 * 10, 50);
        static Object ground(ground2Size, ground2Size, sf::Color::Blue);
        ground.rb.useGravity = false;
        ground.rb.mass = 0;
        ground.setPosition(Vector2(250, 650));

        static Object ground2(ground2Size, ground2Size, sf::Color::Blue);
        ground2.rb.useGravity = false;
        ground2.rb.mass = 0;
        ground2.setPosition(Vector2(250, 350));

        GlobalVars::entities.push_back(&player);
        GlobalVars::entities.push_back(&ground);
        GlobalVars::entities.push_back(&ground2);
    }
    void onEvent(sf::Event e) override {
        if (Utils::keyPressed(e, sf::Keyboard::N)) {
            player.rb.addForce(Vector2(10.f, -50.f * 0.f));
        }
    }
    void onUpdate() override {

    }
    void onDrawGraphs() override {
        auto pos = player.getSprite().getPosition();
        auto vel = player.rb.velocity;
        graphsManager.addPoint(0, time, vel.x);
        graphsManager.addPoint(1, time, vel.y);
    }
};

#endif //PHYSICSSIMULATIONS_PROJECTILE_H
