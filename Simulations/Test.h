#ifndef PHYSICSSIMULATIONS_TEST_H
#define PHYSICSSIMULATIONS_TEST_H

#include "Simulations.h"

using namespace Simulations;

class TestSimulation : public Simulation {
private:
    Vector2 size = Vector2(50, 50);
    Object player = Object(size, size);
public:
    void onCreate() override {
        player.setPosition(Vector2(size.x/2, size.y/2) + Vector2(40, -0));

        auto groundSize = Vector2(500, 50);
        static Object ground(groundSize, groundSize, sf::Color::Green);
        ground.rb.useGravity = false;
        ground.setRotation(20);
        ground.setPosition(Vector2(-100, 300));

        auto ground2Size = Vector2(500 * 10, 50);
        static Object ground2(ground2Size, ground2Size, sf::Color::Blue);
        ground2.rb.useGravity = false;
        ground2.setPosition(Vector2(250, 450));

        GlobalVars::entities.push_back(&player);
        //GlobalVars::entities.push_back(&ground);
        GlobalVars::entities.push_back(&ground2);
    }
    void onEvent(sf::Event e) override {
        if (e.type == sf::Event::KeyPressed) {
            if (e.key.code == sf::Keyboard::N) {
                player.rb.addForce(Vector2(50, 0));
            }
        }
    }
    void onUpdate() override {

    }
    void onDrawGraphs() override {
        auto vel = player.rb.velocity.magnitude();
        std::cout << player.rb.velocity << " " << vel << std::endl;
        graphsManager.addPoint(0, time, vel);
    }
};

#endif //PHYSICSSIMULATIONS_TEST_H
