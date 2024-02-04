#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Object/Object.h"
#include "GraphsManager/GraphsManager.h"
#define SIMULATION 2

#if SIMULATION == 1
#include "Simulations/Test.h"
Simulations::Simulation* simulation = new TestSimulation();
#elif SIMULATION == 2
#include "Simulations/RopeTest.h"
Simulations::Simulation* simulation = new RopeTestSimulation();
#endif

float GlobalVars::deltaTime = 0.f;
std::vector<Entity*> GlobalVars::entities;

bool everySeconds(sf::Clock* clock, float seconds)
{
    if ((float)clock->getElapsedTime().asMilliseconds() > (seconds * 1000))
    {
        clock->restart();
        return true;
    }
    return false;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1500, 750), "Test");
    window.setFramerateLimit(60);

    sf::Text txtFPS;
    sf::Font font;
    bool textEnabled = font.loadFromFile("arial.ttf");
    if (textEnabled)
    {
        txtFPS.setFont(font);
        txtFPS.setCharacterSize(40);
        txtFPS.setFillColor(sf::Color::White);
    }

    sf::Clock updateGraph;

    simulation->onCreate();

    auto rope = (Rope*)GlobalVars::entities[0];
    auto ropeRb = rope->TryGetBehavior<RopeRigidBody>();


    //auto obj = (Object*)GlobalVars::entities[0];
    //auto rb = obj->TryGetBehavior<RigidBody>();
    /*sf::VertexArray line(sf::LinesStrip, 2);
    auto fPos = pos + ropeRb->velocity;
    auto startPos = sf::Vector2f(150, 150);
    line[0].position = startPos;
    line[0].color = sf::Color::Red;
    line[1].color = sf::Color::Red;*/

    while (window.isOpen())
    {
        GlobalVars::deltaTime = simulation->deltaTimeClock.restart().asSeconds();
        simulation->time = (double) simulation->runTimeClock.getElapsedTime().asMilliseconds();
        float fps = 1.f / (GlobalVars::deltaTime);
        if (textEnabled)
            txtFPS.setString(std::to_string((int)fps));
            //txtFPS.setString(std::to_string(ropeRb->velocity.magnitude()));


        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    GlobalVars::entities.clear();
                    simulation->clock.restart();
                    simulation->start = false;
                    simulation->onCreate();
                }
                if (event.key.code == sf::Keyboard::P)
                {
                    simulation->paused = !simulation->paused;
                }
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
            simulation->onEvent(event);
        }

        window.clear();

        if (everySeconds(&simulation->clock, 3))
        {
            if (!simulation->start)
            {
                simulation->runTimeClock.restart();
                simulation->start = true;
            }
        }

        simulation->onUpdate();

        if (simulation->start && !simulation->paused) {
            for (auto entity: GlobalVars::entities) {
                entity->update();
            }
        }

        //line[1].position = startPos + (sf::Vector2f)rope->distance().normalize() * 100.f;
        auto pos = (Vector2)rope->members.end().operator--()->getSprite().getPosition();
        auto r = rope->distance().normalize();
        auto direction = Vector2{-r.y, r.x}.normalize();
        struct Debug::Line line;
        line.start = pos;
        line.direction = ropeRb->velocity.normalize();
        line.distance = ropeRb->velocity.magnitude();
        Debug::drawLine(&window, line);

        if (simulation->start && !simulation->paused) {
            simulation->onDrawGraphs();
        }

        //obj
        for (auto entity: GlobalVars::entities) {
            entity->render(&window);
        }

        if (textEnabled)
            window.draw(txtFPS);

        if (everySeconds(&updateGraph, 0.05f))
        {
            simulation->graphsManager.render();
        }
        window.display();
    }

    return 0;
}