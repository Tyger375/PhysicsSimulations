#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Object.h"

float GlobalVars::deltaTime = 0.f;
int GlobalVars::size = 1;
sf::RectangleShape* GlobalVars::grounds = new sf::RectangleShape[GlobalVars::size];


int main() {
    sf::RenderWindow window(sf::VideoMode(500, 500), "Test");
    //window.setFramerateLimit(120);

    sf::Clock deltaTimeClock;

    auto size = sf::Vector2f(50, 50);
    Object obj(size, size);

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
