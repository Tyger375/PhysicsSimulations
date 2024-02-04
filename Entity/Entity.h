#ifndef PHYSICSSIMULATIONS_ENTITY_H
#define PHYSICSSIMULATIONS_ENTITY_H

#include <SFML/Graphics/RenderWindow.hpp>
#include "../Behaviors/Behavior.h"
#include "../main.h"

class Behavior;

class Entity {
public:
    int id;

    Entity();

    template <class T> T* TryGetBehavior() {
        for (auto behavior : behaviors) {
            T* a = dynamic_cast<T*>(behavior);
            if (a != nullptr) {
                return a;
            }
        }
        return nullptr;
    }

    void AddBehavior(Behavior* b) {
        behaviors.push_back(b);
    }

    virtual void update() = 0;
    virtual void beforeFixedUpdate() = 0;
    virtual void fixedUpdate() = 0;
    virtual void checkCollisions() = 0;
    virtual void render(sf::RenderWindow*) = 0;
protected:
    std::vector<Behavior*> behaviors;
};

#endif //PHYSICSSIMULATIONS_ENTITY_H