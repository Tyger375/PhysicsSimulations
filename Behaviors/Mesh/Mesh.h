#ifndef PHYSICSSIMULATIONS_MESH_H
#define PHYSICSSIMULATIONS_MESH_H

#include <SFML/Graphics.hpp>
#include "../../Vectors/Vector2.h"
#include "../Behavior.h"

class Mesh : public Behavior {
public:
    sf::Shape* shape;

    explicit Mesh(sf::Shape* shape)
        :   Behavior(nullptr),
            shape(shape)
    {
    }

    void update() override {}

    inline void draw(sf::RenderWindow* window) const
    {
        window->draw(*shape);
    }

    static Mesh RectangleMesh(Vector2 size, sf::Color color);
    static Mesh CircleMesh(float radius, sf::Color color);
};


#endif //PHYSICSSIMULATIONS_MESH_H
