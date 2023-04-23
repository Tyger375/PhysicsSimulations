#ifndef PHYSICSSIMULATIONS_ROPE_H
#define PHYSICSSIMULATIONS_ROPE_H

#include <SFML/Graphics.hpp>
#include "../Behaviors/CircleShape/CircleShape.h"
#include "../main.h"
#include <cmath>
#include <functional>

class RopeMember
{
    sf::CircleShape shape;
    CircleShape collision;
public:
    RopeMember(Vector2 pos, float radius, sf::Color c=sf::Color::Yellow)
        : collision(&shape, radius)
    {
        shape.setRadius(radius);
        shape.setPosition((sf::Vector2f) pos);
        shape.setFillColor(c);
    }

    void setPosition(Vector2 pos)
    {
        shape.setPosition((sf::Vector2f) pos);
        collision.update();
    }
    sf::CircleShape getSprite() { return shape; }
    sf::CircleShape* pointer() { return &shape; }
};

class Rope {
private:
    const float radius = 5;
    const Vector2 length;
public:
    std::vector<RopeMember> members;

    Rope(Vector2 start, Vector2 end) : length((end - start))
    {
        resize(start, end);
    }

    void resize(Vector2 start, Vector2 end)
    {
        auto l = end-start;
        auto direction = l.normalize();
        auto distance = l.magnitude();
        int num = (int) (distance / (radius*2));
        float offset = std::fmod(distance, radius * 2) / (float) (num-1);
        num--;
        members.clear();

        members.emplace_back(start, radius, sf::Color::White);

        auto pos = start;
        for (int i = 0; i < num; i++)
        {
            pos += direction * (radius*2 + offset);
            members.emplace_back(pos, radius);
        }

        members.emplace_back(end, radius,  sf::Color::Magenta);
    }

    std::function<double(double)> getParabola()
    {
        //Constructing a catenary https://math.stackexchange.com/questions/3557767/how-to-construct-a-catenary-of-a-specified-length-through-two-specified-points
        auto start = (Vector2)members[0].getSprite().getPosition();
        auto end = (Vector2) members.end().operator--()->getSprite().getPosition();

        if (end.x < start.x)
        {
            auto a = start;
            auto b = end;
            start = b;
            end = a;
        }

        auto l = length.magnitude();

        auto dx = end.x - start.x;
        auto xb = (end.x + start.x)/2;

        if (dx == 0)
            return [start](double Y) {
                return start.x;
            };

        auto dy = end.y - start.y;

        auto r = sqrt(pow(l, 2) - pow(dy, 2)) / dx;

        auto A= 0.01;
        auto dA= 0.0001;
        auto left= r*A;
        auto right= sinh(A);

        while (left >= right)
        {
            left = r * A;
            right = sinh(A);
            A = A + dA;
        }

        A -= dA;

        auto a = -(dx/(2*A));
        auto b = (xb-a*atanh(dy/l));
        auto c = (start.y-a*cosh((start.x-b)/a));

        return [a, b, c](double x) {
            return a*cosh((x-b)/a)+c;
        };
    }

    void render(sf::RenderWindow* window, bool update=false)
    {
        auto startPos = members[0].getSprite().getPosition();
        auto endPos = (Vector2) members.end().operator--()->getSprite().getPosition();


        if (update)
        {
            auto f = getParabola();

            auto l = endPos-startPos;
            int size = (int) members.size();

            for (int i = 1; i < size-1; i++)
            {
                auto *member = &members[i];

                float xPos;
                float y;

                if (startPos.x == endPos.x)
                {
                    float offset = length.magnitude() / (float)(size-1);
                    y = startPos.x + offset * (float)i;
                    xPos = (float) f(y);
                }
                else
                {
                    float offset = l.x / ((float)(size-1));
                    xPos = startPos.x + offset * (float)(i);
                    y = (float) f(xPos);
                }

                member->setPosition(Vector2(xPos, y));
            }
        }

        for (auto member : members)
        {
            window->draw(member.getSprite());
        }
    }

    void setEnd(Vector2 pos)
    {
        auto startPos = (Vector2) members.begin()->getSprite().getPosition();

        auto l = pos - startPos;
        if (l.magnitude() > length.magnitude())
        {
            auto c = l.normalize() * length.magnitude();
            pos = (Vector2)(startPos) + c;
        }

        members.end().operator--()->setPosition(pos);
    }
};


#endif //PHYSICSSIMULATIONS_ROPE_H
