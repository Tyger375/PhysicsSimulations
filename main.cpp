#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Object.h"
#include "Vector2/Vector2.h"

float GlobalVars::deltaTime = 0.f;
int GlobalVars::size = 1;
sf::RectangleShape* GlobalVars::grounds = new sf::RectangleShape[GlobalVars::size];

int main() {
    sf::RenderWindow window(sf::VideoMode(500, 500), "Test");
    window.setFramerateLimit(200);

    sf::Clock deltaTimeClock;

    auto size = Vector2(50, 50);
    Object obj(size, size);
    obj.setPosition(Vector2(size.x/2, size.y/2) + Vector2(40, 0));


    auto groundSize = Vector2(500, 50);
    Object ground(groundSize, groundSize, sf::Color::Green);
    ground.setRotation(0);
    ground.setPosition(Vector2(250, 400));


    sf::Text txtFPS;
    sf::Font font;
    bool textEnabled = font.loadFromFile("arial.ttf");
    if (textEnabled)
    {
        txtFPS.setFont(font);
        txtFPS.setCharacterSize(40);
        txtFPS.setFillColor(sf::Color::White);
    }

    GlobalVars::grounds[0] = ground.getSprite();

    sf::Clock clock;
    sf::Time elapsedTime;

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
        if (elapsedTime.asMilliseconds() > 2000)
        {
            //obj.rb.addForce(Vector2f(0, -1.f) * 9.81f * 20.f, FORCE);
            elapsedTime = sf::milliseconds(0);
        }

        obj.update();
        window.draw(obj.getSprite());
        //window.draw(obj.debugBounds());

        window.draw(ground.getSprite());
        //window.draw(ground.debugBounds());

        if (textEnabled)
            window.draw(txtFPS);

        window.display();
    }

    return 0;
}
