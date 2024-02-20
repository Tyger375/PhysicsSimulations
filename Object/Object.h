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

    Object(Mesh mesh, CollisionShape* collisionShape)
        : mesh(mesh),
          shape(collisionShape),
          rb(mesh.shape, this, shape, CONTINUOUS)
    {
        this->mesh.entity = this;
        this->shape->entity = this;
        AddBehavior(&this->mesh);
        AddBehavior(shape);
        AddBehavior(&rb);
    }

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

    inline void update() override
    {

    }

    inline void beforeFixedUpdate() override {
        rb.updateVars();
    }

    inline void checkCollisions() override {
        rb.checkCollisions((Vector2)this->mesh.shape->getPosition());
    }

    inline void fixedUpdate() override
    {
        rb.update();
        shape->update();
    }

    void render(sf::RenderWindow* window) override;

    [[nodiscard]] inline Mesh getSprite() const { return mesh; }
    [[nodiscard]] inline CollisionShape* getBounds() const { return shape; }
};


#endif //PHYSICSSIMULATIONS_OBJECT_H
