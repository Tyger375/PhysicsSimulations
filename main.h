#ifndef PHYSICSSIMULATIONS_MAIN_H
#define PHYSICSSIMULATIONS_MAIN_H

namespace GlobalVars
{
    extern float deltaTime;
    extern sf::RectangleShape* grounds;
    extern int size;
}

namespace math
{
    static float lerp(float a, float b, float f)
    {
        return a * (1.0f - f) + (b * f);
    }

    static double degToRad(float angle)
    {
        return (angle * M_PI) / 180;
    }
}

#endif //PHYSICSSIMULATIONS_MAIN_H
