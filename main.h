#ifndef PHYSICSSIMULATIONS_MAIN_H
#define PHYSICSSIMULATIONS_MAIN_H

#include "Behaviors/CollisionShape/CollisionShape.h"

namespace GlobalVars
{
    extern float deltaTime;
    extern std::vector<CollisionShape*> grounds;
    extern int size;
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

#endif //PHYSICSSIMULATIONS_MAIN_H
