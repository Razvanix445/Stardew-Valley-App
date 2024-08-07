#ifndef ENTITY_H
#define ENTITY_H

class Entity {

protected:
    long id;

public:
    // Constructor
    Entity();
    Entity(const long id);

    // Get the Entity ID
    long getId() const;

    // Set the Entity ID to a new value
    void setId(const long id);
};

#endif // ENTITY_H
