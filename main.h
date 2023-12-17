#ifndef PHYSICSSIMULATIONS_MAIN_H
#define PHYSICSSIMULATIONS_MAIN_H

#include <Math/Math.h>
#include "Vector2/Vector2.h"
#include <SFML/Graphics.hpp>

class Entity;
namespace GlobalVars
{
    extern int lastId;
    extern float deltaTime;
    extern std::vector<Entity*> entities;
}

namespace math
{
    static double lerp(double a, double b, double f)
    {
        //Lerp faster by NVIDIA https://developer.nvidia.com/blog/lerp-faster-cuda/
        return fma(f, b, fma(-f, a, a));
    }

    static double degToRad(float angle)
    {
        return (angle * M_PI) / 180;
    }

    static float sign(float x)
    {
        return (x > 0) ? 1.f : ((x < 0) ? -1.f : 0.f);
    }
}

namespace Debug {
    struct Line {
        Vector2 start;
        Vector2 direction;
        float distance{};
    };
    static void drawLine(sf::RenderWindow* window, struct Line l)
    {
        sf::VertexArray line(sf::LineStrip, 2);
        line[0].color = sf::Color::Red;
        line[1].color = sf::Color::Red;

        line[0].position = (sf::Vector2f)l.start;
        line[1].position = (sf::Vector2f)(l.start + l.direction * l.distance);
        window->draw(line);
    }
}

#endif //PHYSICSSIMULATIONS_MAIN_H
