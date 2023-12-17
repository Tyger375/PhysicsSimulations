#include "Entity.h"

int GlobalVars::lastId = 0;

Entity::Entity() {
    id = ++GlobalVars::lastId;
}