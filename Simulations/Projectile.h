#ifndef PHYSICSSIMULATIONS_PROJECTILE_H
#define PHYSICSSIMULATIONS_PROJECTILE_H

#include "Simulations.h"
#include "Object/Object.h"
#include "../Utils.h"

using namespace Simulations;

class ProjectileSimulation : public Simulation {
private:
    Vector2 size = Vector2(50, 50);
    Object player = Object(size, size);

    Object box = Object(size + Vector2(50, 0), size + Vector2(50, 0), sf::Color::Magenta);

    Vector2 ground2Size = Vector2(300, 50);
    Object ground = Object(ground2Size, ground2Size, sf::Color::Blue);
public:
    void onCreate() override {
        //Graphs
        graphsManager.addGraph("Graph");
        graphsManager.addGraph("Graph");
        graphsManager.build();

        player.rb.useGravity = true;
        player.rb.setDensity(10);
        player.setPosition(Vector2(100, 500));
        std::cout << player.id << std::endl;
        //player.setRotation(10);
        //player.rb.addForce(Vectors(100.f, -50.f * 0.f));

        ground.rb.useGravity = false;
        ground.rb.setDensity(0);
        ground.setPosition(Vector2(160, 650));
        //ground.setRotation(10);

        box.rb.useGravity = true;
        box.rb.setDensity(50);
        box.setPosition(Vector2(50, 570));

        static Object ground2(ground2Size, ground2Size, sf::Color::Blue);
        ground2.rb.useGravity = false;
        ground2.rb.setDensity(0);
        ground2.setPosition(Vector2(250, 350));

        auto groundSize = Vector2(50, 300);
        static Object verticalGround(groundSize, groundSize, sf::Color::Green);
        verticalGround.rb.useGravity = false;
        verticalGround.rb.setDensity(0);
        verticalGround.setPosition(Vector2(700, 550));

        GlobalVars::entities.push_back(&box);
        GlobalVars::entities.push_back(&player);
        GlobalVars::entities.push_back(&ground);
        //GlobalVars::entities.push_back(&ground2);
        //GlobalVars::entities.push_back(&verticalGround);

        std::cout << ground.rb.inv_mass << std::endl;
    }
    void onEvent(sf::Event e) override {
        if (Utils::keyPressed(e, sf::Keyboard::N)) {
            player.rb.addForce(Vector2(100.f, -50.f * 0.f));
        }
    }
    void onUpdate() override {

    }
    void onRender(sf::RenderWindow* window) override {
        //player.rb.debug(window);
    }
    void onDrawGraphs() override {
        graphsManager.addPoint(1, time, player.rb.velocity.magnitude());
        graphsManager.addPoint(0, time, player.rb.angularVelocity);
    }
};

#endif //PHYSICSSIMULATIONS_PROJECTILE_H
