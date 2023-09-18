#ifndef PHYSICSSIMULATIONS_BEHAVIOR_H
#define PHYSICSSIMULATIONS_BEHAVIOR_H

class Behavior {
public:
    virtual ~Behavior() = default;

    virtual void update() = 0;
};

#endif //PHYSICSSIMULATIONS_BEHAVIOR_H
