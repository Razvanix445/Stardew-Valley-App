#include "Entity.h"

Entity::Entity(const long id) : id(id) {}
Entity::Entity() : id(0) {}

const long Entity::getId() const {
    return id;
}

void Entity::setId(const long id) {
    this->id = id;
}
