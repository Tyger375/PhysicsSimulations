#ifndef PHYSICSSIMULATIONS_ROPE_H
#define PHYSICSSIMULATIONS_ROPE_H

#include <SFML/Graphics.hpp>
#include "../Behaviors/CircleShape/CircleShape.h"
#include "../main.h"
#include "../Entity.h"
#include "RopeMember.h"
#include <cmath>
#include <functional>

class Rope : public Entity {
private:
    const float radius = 5;
public:
    std::vector<RopeMember> members;

    Rope(Vector2 start, Vector2 end) : length((end - start))
    {
        resize(start, end);
    }

    void resize(Vector2 start, Vector2 end);

    std::function<double(double)> getParabola();

    void render(sf::RenderWindow*) override;

    void update() override;

    void setEnd(Vector2 pos);

    Vector2 distance()
    {
        return
            (members.end().operator--()->getSprite().getPosition()
            -
            members.begin()->getSprite().getPosition());
    }

    float getTension()
    {
        //1:length = x:distance
        const int precision = 2;
        const auto mult = (float) pow(10, precision);
        auto value = (float)(round((double)distance().magnitude()/length.magnitude() * mult)) / mult;
        return std::clamp(value, 0.f, 1.f);
    }

    const Vector2 length;
};


#endif //PHYSICSSIMULATIONS_ROPE_H
