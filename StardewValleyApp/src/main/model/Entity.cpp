#include "Entity.h"

Entity::Entity() : id(0) {}
Entity::Entity(const long id) : id(id) {}

long Entity::getId() const {
    return id;
}

void Entity::setId(const long id) {
    this->id = id;
}
