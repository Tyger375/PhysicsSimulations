#ifndef PHYSICSSIMULATIONS_BEHAVIOR_H
#define PHYSICSSIMULATIONS_BEHAVIOR_H

#include "../Entity/Entity.h"

class Entity;

class Behavior {
public:
    Entity* entity;
    explicit Behavior(Entity* parent) : entity(parent) {}
    virtual ~Behavior() = default;

    virtual void update() = 0;
};

#endif //PHYSICSSIMULATIONS_BEHAVIOR_H
