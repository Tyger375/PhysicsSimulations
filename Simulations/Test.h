#ifndef PHYSICSSIMULATIONS_TEST_H
#define PHYSICSSIMULATIONS_TEST_H

#include "Simulations.h"
#include "../Vectors/Vector2.h"
#include "../Object/Object.h"
#include "../Utils.h"

using namespace Simulations;

class TestSimulation : public Simulation {
private:
    Vector2 size = Vector2(25, 25);
    Mesh playerMesh = Mesh::RectangleMesh(size, sf::Color::Red);
    RectangleShape playerShape = RectangleShape(&playerMesh, size);
    Object player = Object(playerMesh, &playerShape);
public:
    explicit TestSimulation(GraphsManager* m) : Simulation(m) {}
    void onCreate() override {
        graphsManager.setCanvasSize(sf::Vector2i(1550, 700));
        graphsManager.setCanvasPosition(sf::Vector2i(-2048, 0));

        graphsManager.addGraph("Test");
        graphsManager.build();

        player.rb.setDensity(1);
        player.setPosition(Vector2(550, 350));
        player.setRotation(15);

        auto groundSize = Vector2(500, 50);
        Mesh groundMesh = Mesh::RectangleMesh(groundSize, sf::Color::Green);
        auto* groundShape = new RectangleShape(&groundMesh, groundSize);
        static Object ground(groundMesh, groundShape);
        ground.rb.useGravity = false;
        ground.rb.setDensity(0);
        ground.setPosition(Vector2(250, 450));

        /*
        auto ground2Size = Vector2(500 * 10, 50);
        static Object ground2(ground2Size, ground2Size, sf::Color::Blue);
        ground2.rb.useGravity = false;
        ground2.rb.setDensity(0);
        ground2.setPosition(Vector2(250, 450));

        //GlobalVars::entities.push_back(&ground);
        GlobalVars::entities.push_back(&ground2);*/
        GlobalVars::entities.push_back(&player);
        GlobalVars::entities.push_back(&ground);
    }
    void onEvent(sf::Event e) override {
        if (Utils::keyPressed(e, sf::Keyboard::N)) {
            player.rb.addForce(Vector2(500, 0), IMPULSE);
        }
    }
    void onUpdate() override {
    }
    void onRender(sf::RenderWindow* window) override {
        player.rb.debug(window);
    }
    void onDrawGraphs() override {
    }
};

#endif //PHYSICSSIMULATIONS_TEST_H
