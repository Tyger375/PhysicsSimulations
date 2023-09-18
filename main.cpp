#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Object.h"
#include "Vector2/Vector2.h"
#include "Rope/Rope.h"
#include "Rope/RopeRigidBody.h"
#include "Simulations.h"

float GlobalVars::deltaTime = 0.f;
std::vector<Entity*> GlobalVars::entities;

int main() {
    sf::RenderWindow window(sf::VideoMode(500, 500), "Test");
    window.setFramerateLimit(200);

    sf::Clock deltaTimeClock;

    //Simulations::Test();
    //Simulations::RopeTest();
    Simulations::Projectile();

    sf::Text txtFPS;
    sf::Font font;
    bool textEnabled = font.loadFromFile("arial.ttf");
    if (textEnabled)
    {
        txtFPS.setFont(font);
        txtFPS.setCharacterSize(40);
        txtFPS.setFillColor(sf::Color::White);
    }

    sf::Clock clock;
    sf::Time elapsedTime;

    auto start = false;

    while (window.isOpen())
    {
        GlobalVars::deltaTime = deltaTimeClock.restart().asSeconds();
        float fps = 1.f / (GlobalVars::deltaTime);
        if (textEnabled)
            txtFPS.setString(std::to_string((int)fps));


        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

        }

        window.clear();

        //Do something every 2 seconds
        elapsedTime += clock.getElapsedTime();
        clock.restart();
        if (elapsedTime.asMilliseconds() > 1000)
        {
            //obj.rb.addForce(Vector2f(0, -1.f) * 9.81f * 20.f, FORCE);
            //obj.setPosition(Vector2(25, 25));
            //rope.setEnd(Vector2(100, 20));
            start = true;
            elapsedTime = sf::milliseconds(0);
        }

        if (start) {
            for (auto entity: GlobalVars::entities) {
                entity->update();
            }
        }

        //obj

        for (auto entity: GlobalVars::entities) {
            entity->render(&window);
        }

        if (textEnabled)
            window.draw(txtFPS);

        window.display();
    }

    return 0;
}
