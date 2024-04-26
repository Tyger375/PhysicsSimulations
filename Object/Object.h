#ifndef PHYSICSSIMULATIONS_OBJECT_H
#define PHYSICSSIMULATIONS_OBJECT_H


#include <SFML/Graphics.hpp>
#include "../Behaviors/RigidBody/RigidBody.h"
#include "../Behaviors/CollisionShape/CollisionShape.h"
#include "../Behaviors/RectangleShape/RectangleShape.h"
#include "../Entity/Entity.h"
#include "../Behaviors/Mesh/Mesh.h"

class Object : public Entity {
public:
    Mesh mesh;
    CollisionShape* shape;
    RigidBody rb;

    Object(const Mesh& mesh, CollisionShape* collisionShape);

    explicit Object(Vector2 size);

    Object();

    inline void setPosition(Vector2 pos) const
    {
        mesh.shape->setPosition((sf::Vector2f) pos);
        shape->update();
    }

    inline void setRotation(float rot) const
    {
        mesh.shape->setRotation(rot);
        shape->update();
    }

    inline void update() override {}

    void beforeFixedUpdate() override;

    void checkCollisions() override;

    void fixedUpdate() override;

    void render(sf::RenderWindow* window) override;

    [[nodiscard]] inline Mesh getSprite() const { return mesh; }
    [[nodiscard]] inline CollisionShape* getBounds() const { return shape; }
};


#endif //PHYSICSSIMULATIONS_OBJECT_H
