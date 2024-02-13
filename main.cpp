#if SIMULATION == 1
#include "Simulations/Test.h"
Simulations::Simulation* simulation = new TestSimulation();
#elif SIMULATION == 2
#include "Simulations/RopeTest.h"
Simulations::Simulation* simulation = new RopeTestSimulation();
#elif SIMULATION == 3
#include "Simulations/Projectile.h"
Simulations::Simulation* simulation = new ProjectileSimulation();
#endif

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Entity/Entity.h"
#include "Utils.h"

double GlobalVars::deltaTime = 0.f;
double GlobalVars::fixedDeltaTime = 0.f;
std::vector<Entity*> GlobalVars::entities;

int main() {
    sf::RenderWindow window(sf::VideoMode(1024, 500), "Test");
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

    // Implementing fixed looping
    sf::Clock clock;
    GlobalVars::fixedDeltaTime = 1.0 / 40.0;
    double accumulator = 0;

    //TODO: Better performance

    while (window.isOpen())
    {
        GlobalVars::deltaTime = simulation->deltaTimeClock.restart().asSeconds();
        simulation->time = (double) simulation->runTimeClock.getElapsedTime().asMilliseconds();
        double fps = 1.0 / GlobalVars::deltaTime;

        if (textEnabled)
            txtFPS.setString(std::to_string((int)fps));

        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (Utils::keyPressed(event, sf::Keyboard::Space))
            {
                GlobalVars::entities.clear();
                simulation->start = false;
                simulation->onCreate();
            }
            if (Utils::keyPressed(event, sf::Keyboard::P))
                simulation->paused = !simulation->paused;
            if (Utils::keyPressed(event, sf::Keyboard::Escape))
                window.close();

            simulation->onEvent(event);
        }

        if (Utils::everySeconds(&simulation->clock, 3))
        {
            if (!simulation->start)
            {
                simulation->runTimeClock.restart();
                simulation->start = true;
            }
        }

        accumulator += GlobalVars::deltaTime;

        while (accumulator >= GlobalVars::fixedDeltaTime) {
            accumulator -= GlobalVars::fixedDeltaTime;
            if (simulation->start && !simulation->paused) {
                for (auto entity: GlobalVars::entities) {
                    entity->beforeFixedUpdate();
                }

                for (auto entity : GlobalVars::entities) {
                    entity->checkCollisions();
                }

                for (auto entity : GlobalVars::entities) {
                    entity->fixedUpdate();
                }
            }
        }

        simulation->onUpdate();

        if (simulation->start && !simulation->paused) {
            for (auto entity: GlobalVars::entities) {
                entity->update();
            }
        }

        if (simulation->start && !simulation->paused) {
            simulation->onDrawGraphs();
        }

        window.clear();

        // rendering entities
        for (auto entity: GlobalVars::entities) {
            entity->render(&window);
        }

        if (textEnabled)
            window.draw(txtFPS);

        simulation->onRender(&window);

        if (Utils::everySeconds(&updateGraph, 0.1f))
        {
            simulation->graphsManager.render();
        }
        window.display();
    }

    return 0;
}