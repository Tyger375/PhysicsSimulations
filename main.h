#ifndef PHYSICSSIMULATIONS_MAIN_H
#define PHYSICSSIMULATIONS_MAIN_H

#include <iostream>
#include <Math/Math.h>
#include "Vectors/Vector2.h"
#include <SFML/Graphics.hpp>

class Entity;
namespace GlobalVars
{
    extern int lastId;
    extern double deltaTime;
    extern double fixedDeltaTime;
    extern std::vector<Entity*> entities;
}

struct ContactPoint {
    float dist{};
    Vector2 point;
    bool contains{};
};

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

    static double radToDeg(double angle)
    {
        return (angle * 180) / M_PI;
    }

    static float sign(float x)
    {
        return (x > 0) ? 1.f : ((x < 0) ? -1.f : 0.f);
    }

    static void getLine(Vector2 p1, Vector2 p2, double &a, double &b, double &c)
    {
        // (x- p1X) / (p2X - p1X) = (y - p1Y) / (p2Y - p1Y)
        a = p1.y - p2.y;
        b = p2.x - p1.x;
        c = p1.x * p2.y - p2.x * p1.y;
    }

    static ContactPoint dist(Vector2 p, Vector2 start, Vector2 end)
    {
        auto edge = end - start;
        auto pointDir = p - start;
        auto proj = pointDir.dot(edge);
        auto contains = proj >= 0 && proj <= edge.dot(edge);

        double a, b, c;
        // ax + by + c = 0

        getLine(start, end, a, b, c);

        auto distance = (float)(abs(a * p.x + b * p.y + c) / sqrt(a * a + b * b));
        // Calculate the coordinates of the closest point on the line
        auto closestPointX = (float)((b * (b * p.x - a * p.y) - a * c) / (a * a + b * b));
        auto closestPointY = (float)((a * (-b * p.x + a * p.y) - b * c) / (a * a + b * b));

        return ContactPoint{distance, Vector2(closestPointX, closestPointY), contains};
    }

    static std::pair<Vector2, Vector2> getBounds(Vector2 size, Vector2 position, Vector2 velocity) {
        auto width = size.x;
        auto height = size.y;

        auto dir = velocity.normalize();
        auto s = Vector2{dir.x * width / 2.f, dir.y * height / 2.f};

        auto start = position - s;
        auto end = position + velocity + s;

        return {start, end};
    }
}

namespace Debug {
    struct Line {
        Vector2 position;
        Vector2 direction;
        float distance{};
        sf::Color color = sf::Color::Red;
    };
    struct Point {
        Vector2 position;
        sf::Color color = sf::Color::Red;
    };
    static void drawLine(sf::RenderWindow* window, struct Line l)
    {
        sf::VertexArray line(sf::LineStrip, 2);
        line[0].color = l.color;
        line[1].color = l.color;

        line[0].position = (sf::Vector2f)l.position;
        line[1].position = (sf::Vector2f)(l.position + l.direction * l.distance);
        window->draw(line);
    }
    static void drawPoint(sf::RenderWindow* window, struct Point p)
    {
        sf::CircleShape point;
        point.setRadius(2.f);
        point.setPosition((sf::Vector2f) p.position);
        point.setFillColor(p.color);
        window->draw(point);
    }
}

#endif //PHYSICSSIMULATIONS_MAIN_H
