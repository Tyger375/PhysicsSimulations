#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Behaviors/RigidBody/RigidBody.h"
#include "Behaviors/CollisionShape/CollisionShape.h"
#include "Object.h"

float GlobalVars::deltaTime = 0.f;
int GlobalVars::size = 1;
sf::RectangleShape* GlobalVars::grounds = new sf::RectangleShape[GlobalVars::size];


int main() {
    sf::RenderWindow window(sf::VideoMode(500, 500), "Test");
    window.setFramerateLimit(30);

    sf::Clock deltaTimeClock;

    Object obj;

    sf::RectangleShape ground;
    ground.setSize(sf::Vector2f(500, 50));
    ground.setFillColor(sf::Color::Green);
    ground.setPosition(0, 450);

    sf::Text txtFPS;
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        return 0;
    }
    txtFPS.setFont(font);
    txtFPS.setCharacterSize(40);
    txtFPS.setFillColor(sf::Color::White);

    GlobalVars::grounds[0] = ground;

    sf::Clock clock;
    sf::Time elapsedTime;

    while (window.isOpen())
    {
        GlobalVars::deltaTime = deltaTimeClock.restart().asSeconds();
        float fps = 1.f / (GlobalVars::deltaTime);
        txtFPS.setString(std::to_string((int)fps));

        elapsedTime += clock.getElapsedTime();
        clock.restart();
        if (elapsedTime.asMilliseconds() > 1000)
        {
            std::cout << obj.getSprite().getPosition().y << std::endl;
            elapsedTime = sf::milliseconds(0);
        }


        //std::cout << deltaTime << std::endl;
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

        }

        window.clear();
        obj.update();
        window.draw(obj.getSprite());

        window.draw(obj.debugBounds());
        window.draw(ground);
        window.draw(txtFPS);

        window.display();
    }

    return 0;
}
