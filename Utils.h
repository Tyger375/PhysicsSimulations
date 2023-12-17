#ifndef PHYSICSSIMULATIONS_UTILS_H
#define PHYSICSSIMULATIONS_UTILS_H

#include <SFML/Window.hpp>

namespace Utils {
    bool everySeconds(sf::Clock* clock, float seconds)
    {
        if ((float)clock->getElapsedTime().asMilliseconds() > (seconds * 1000))
        {
            clock->restart();
            return true;
        }
        return false;
    }

    bool keyPressed(sf::Event e, sf::Keyboard::Key key) {
        return e.type == sf::Event::KeyPressed && e.key.code == key;
    }
}

#endif //PHYSICSSIMULATIONS_UTILS_H
