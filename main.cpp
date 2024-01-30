#define SIMULATION 3
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

float GlobalVars::deltaTime = 0.f;
float GlobalVars::fixedDeltaTime = 0.f;
std::vector<Entity*> GlobalVars::entities;

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

    //Implementing fixed looping
    sf::Clock clock;
    GlobalVars::fixedDeltaTime = 1.0 / 120.0;
    const sf::Time timePerFrame = sf::seconds(GlobalVars::fixedDeltaTime);
    sf::Time deltaTime = sf::Time::Zero;
    /*
    auto rope = (Rope*)GlobalVars::entities[0];
    auto ropeRb = rope->TryGetBehavior<RopeRigidBody>();
    */

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

        if (Utils::everySeconds(&simulation->clock, 3))
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

        deltaTime += clock.restart();

        while (deltaTime >= timePerFrame) {
            deltaTime -= timePerFrame;
            if (simulation->start && !simulation->paused) {
                for (auto entity: GlobalVars::entities) {
                    entity->fixedUpdate();
                }
            }
        }

        //line[1].position = startPos + (sf::Vector2f)rope->distance().normalize() * 100.f;
        /*auto pos = rope->members.end().operator--()->getSprite().getPosition();
        auto r = rope->distance().normalize();
        auto direction = Vector2{-r.y, r.x}.normalize();
        struct Debug::Line line;
        line.start = pos;
        line.direction = ropeRb->velocity.normalize();
        line.distance = 100.f;
        Debug::drawLine(&window, line);*/

        if (simulation->start && !simulation->paused) {
            simulation->onDrawGraphs();
        }

        window.clear();

        //obj
        for (auto entity: GlobalVars::entities) {
            entity->render(&window);
        }

        if (textEnabled)
            window.draw(txtFPS);

        if (Utils::everySeconds(&updateGraph, 0.1f))
        {
            simulation->graphsManager.render();
        }
        window.display();
    }

    return 0;
}