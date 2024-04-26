#include "Object.h"

Object::Object(const Mesh& mesh, CollisionShape *collisionShape)
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

Object::Object(Vector2 size)
        : mesh(Mesh::RectangleMesh(size, sf::Color::Red)),
          shape(new RectangleShape(&mesh, size)),
          rb(mesh.shape, this, shape, CONTINUOUS)
{
    this->mesh.entity = this;
    this->shape->entity = this;
    AddBehavior(&this->mesh);
    AddBehavior(shape);
    AddBehavior(&rb);
}

Object::Object()
        : mesh(Mesh::RectangleMesh(Vector2(50, 50), sf::Color::Red)),
          shape(new RectangleShape(&mesh, Vector2(50, 50))),
          rb(mesh.shape, this, shape, CONTINUOUS)
{
    this->mesh.entity = this;
    this->shape->entity = this;
    AddBehavior(&this->mesh);
    AddBehavior(shape);
    AddBehavior(&rb);
}

void Object::render(sf::RenderWindow *window) {
    mesh.draw(window);
}

void Object::fixedUpdate() {
    rb.update();
    shape->update();
}

void Object::checkCollisions() {
    rb.checkCollisions((Vector2)this->mesh.shape->getPosition());
}

void Object::beforeFixedUpdate() {
    rb.updateVars();
}