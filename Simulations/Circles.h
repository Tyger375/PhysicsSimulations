#ifndef PHYSICSSIMULATIONS_CIRCLES_H
#define PHYSICSSIMULATIONS_CIRCLES_H

#include "Simulations.h"
#include "../Vectors/Vector2.h"
#include "../Object/Object.h"
#include "../Utils.h"
#include "../Behaviors/CircleShape/CircleShape.h"

using namespace Simulations;

class CirclesSimulation : public Simulation {
private:
    float radius = 15.f;
    Mesh playerMesh = Mesh::CircleMesh(radius, sf::Color::Red);
    CircleShape playerShape = CircleShape(&playerMesh, radius);
    Object player = Object(playerMesh, &playerShape);

    Mesh playerMesh2 = Mesh::CircleMesh(radius, sf::Color::Yellow);
    CircleShape playerShape2 = CircleShape(&playerMesh2, radius);
    Object player2 = Object(playerMesh2, &playerShape2);

    int i = 0;

    void instantiateCircle() {
        Mesh mesh = Mesh::CircleMesh(radius, sf::Color::Red);
        mesh.shape->setOutlineColor(sf::Color::Cyan);
        auto* shape = new CircleShape(&mesh, radius);
        auto* newObj = new Object(mesh, shape);

        newObj->rb.setDensity(10);
        newObj->setPosition(Vector2(400 + (i * 5), 175 + (i * 50)));

        i++;
        GlobalVars::entities.push_back(newObj);
    }

public:
    void onCreate() override {
        graphsManager.setCanvasSize(sf::Vector2i(1550, 700));
        graphsManager.setCanvasPosition(sf::Vector2i(-2048, 0));

        graphsManager.addGraph("Test");
        graphsManager.build();

        player.rb.setDensity(10);
        player.setPosition(Vector2(555, 435));

        player2.rb.setDensity(20);
        player2.setPosition(Vector2(560, 400));

        auto groundSize = Vector2(500, 50);
        Mesh groundMesh = Mesh::RectangleMesh(groundSize, sf::Color::Green);
        auto groundShape = new RectangleShape(&groundMesh, groundSize);
        static Object ground(groundMesh, groundShape);
        ground.rb.useGravity = false;
        ground.rb.setDensity(0);
        ground.setPosition(Vector2(250, 450));

        instantiateCircle();
        instantiateCircle();
        instantiateCircle();

        //ground2.rb.useGravity = false;
        //ground2.rb.setDensity(20);
        //ground2.setPosition(Vector2(500, 355));

        /*
        auto ground2Size = Vector2(500 * 10, 50);
        static Object ground2(ground2Size, ground2Size, sf::Color::Blue);
        ground2.rb.useGravity = false;
        ground2.rb.setDensity(0);
        ground2.setPosition(Vector2(250, 450));

        //GlobalVars::entities.push_back(&ground);
        GlobalVars::entities.push_back(&ground2);*/
        //GlobalVars::entities.push_back(&player);
        GlobalVars::entities.push_back(&player2);
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
        player2.rb.debug(window);
        //ground2.rb.debug(window);
        auto bounds = (sf::CircleShape*)player.getBounds()->getBounds();

        auto point = (Vector2)bounds->getPosition();
        auto r = bounds->getRadius() / 2.f;
        //point += Vector2(r, r);

        Debug::Point p{point, sf::Color::Green};
        //Debug::drawPoint(window, p);
        //window->draw(*player.getBounds()->getBounds());
    }
    void onDrawGraphs() override {
    }
};

#endif //PHYSICSSIMULATIONS_CIRCLES_H
