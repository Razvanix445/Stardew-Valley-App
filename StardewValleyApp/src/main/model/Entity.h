#ifndef ENTITY_H
#define ENTITY_H

#include <qDebug>

class Entity {

protected:
    long id;

public:
    // Constructor
    Entity(const long id);
    Entity();

    // Get the Entity ID
    const long getId() const;

    // Set the Entity ID to a new value
    void setId(const long id);
};

#endif // ENTITY_H
