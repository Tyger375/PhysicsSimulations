#ifndef PHYSICSSIMULATIONS_PROJECTILE_H
#define PHYSICSSIMULATIONS_PROJECTILE_H

#include "Simulations.h"
#include "Object/Object.h"
#include "../Utils.h"

using namespace Simulations;

class ProjectileSimulation : public Simulation {
private:
    Vector2 size = Vector2(50, 50);
    Object player = Object();

    Vector2 boxSize = size + Vector2(50, 0);
    Mesh boxMesh = Mesh::RectangleMesh(size, sf::Color::Magenta);
    RectangleShape boxShape = RectangleShape(&boxMesh, boxSize);
    Object box = Object(boxMesh, &boxShape);

    Vector2 ground2Size = Vector2(500, 50);
    Mesh groundMesh = Mesh::RectangleMesh(ground2Size, sf::Color::Blue);
    RectangleShape groundShape = RectangleShape(&groundMesh, ground2Size);
    Object ground = Object(groundMesh, &groundShape);
public:
    explicit ProjectileSimulation(GraphsManager* m) : Simulation(m) {}

    void onCreate() override {
        //Graphs
        graphsManager->addGraph("Graph", [](TGraph* _){}, GraphMode::WITH_LINES);
        graphsManager->build();

        player.rb.useGravity = true;
        player.rb.setDensity(10);
        player.setPosition(Vector2(70, 250));
        //player.setRotation(10);
        //player.rb.addForce(Vectors(100.f, -50.f * 0.f));

        ground.rb.useGravity = false;
        ground.rb.setDensity(0);
        ground.setPosition(Vector2(0, 450));
        //ground.setRotation(10);

        //box.rb.useGravity = false;
        box.rb.setDensity(10);
        box.setPosition(Vector2(50, 400));

        GlobalVars::entities.push_back(&box);
        GlobalVars::entities.push_back(&player);
        GlobalVars::entities.push_back(&ground);
        //GlobalVars::entities.push_back(&ground2);
        //GlobalVars::entities.push_back(&verticalGround);
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
        graphsManager->addPoint(0, time, player.rb.velocity.magnitude());
    }
};

#endif //PHYSICSSIMULATIONS_PROJECTILE_H
