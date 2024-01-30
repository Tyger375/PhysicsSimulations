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
        //Graphs
        graphsManager.addGraph("Graph");
        graphsManager.addGraph("Graph");
        //graphsManager.addGraph("Graph");
        graphsManager.build();

        player.rb.useGravity = true;
        player.rb.mass = 10.f;
        player.setPosition(Vector2(50, 500));
        player.rb.addForce(Vector2(100.f, -50.f * 0.f));

        auto ground2Size = Vector2(1000, 50);
        static Object ground(ground2Size, ground2Size, sf::Color::Blue);
        ground.rb.useGravity = false;
        ground.rb.mass = 0;
        ground.setPosition(Vector2(50, 650));
        ground.setRotation(10);

        static Object ground2(ground2Size, ground2Size, sf::Color::Blue);
        ground2.rb.useGravity = false;
        ground2.rb.mass = 0;
        ground2.setPosition(Vector2(250, 350));

        static Object box(size + Vector2(50, 0), size + Vector2(50, 0), sf::Color::Magenta);
        box.rb.useGravity = true;
        box.rb.mass = 50;
        box.setPosition(Vector2(50, 570));

        auto groundSize = Vector2(50, 300);
        static Object verticalGround(groundSize, groundSize, sf::Color::Green);
        verticalGround.rb.useGravity = false;
        verticalGround.rb.mass = 0;
        verticalGround.setPosition(Vector2(700, 550));

        //GlobalVars::entities.push_back(&box);
        GlobalVars::entities.push_back(&player);
        GlobalVars::entities.push_back(&ground);
        GlobalVars::entities.push_back(&ground2);
        GlobalVars::entities.push_back(&verticalGround);
    }
    void onEvent(sf::Event e) override {
        if (Utils::keyPressed(e, sf::Keyboard::N)) {
            player.rb.addForce(Vector2(100.f, -50.f * 0.f));
        }
    }
    void onUpdate() override {

    }
    void onDrawGraphs() override {
        auto pos = player.getSprite().getPosition();
        //auto vel = player.rb.velocity;
        auto vec = pos;
        //graphsManager.addPoint(0, time, vec.x);
        //graphsManager.addPoint(0, time, vel.y);
        graphsManager.addPoint(1, time, vec.y);
        float fps = 1.f / (GlobalVars::deltaTime);
        graphsManager.addPoint(0, time, fps);
    }
};

#endif //PHYSICSSIMULATIONS_PROJECTILE_H
