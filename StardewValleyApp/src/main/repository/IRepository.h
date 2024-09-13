#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <memory>
#include <vector>
#include <string>

using namespace std;

/**
 * @brief The IRepository interface
 * CRUD operations
 * Used only for database interactions
 */
template<typename T>
class IRepository {
public:
    /**
     *  Searches and returns an entity from the database by its ID
     *  @param id - the ID of the entity to be returned
     *              ID must not be null
     *  @return the entity with the specified ID
     *          or null if there is no entity with the given ID
     */
    virtual T findOne(long id, const long userId) const = 0;

    /**
     *  @brief Returns all the entities from the database
     *  @return all entities
     */
    virtual vector<T> findAll(const long userId) const = 0;

    /**
     * @brief Saves the given entity in the database
     * @param entity
     */
    virtual void save(T& entity) = 0;

    /**
     * @brief Removes the entity with the specified ID
     * @param id - ID must not be null
     */
    virtual void remove(long id) = 0;

    /**
     * @brief Updates the entity with the new values
     * @param entity
     */
    virtual T update(const T& entity, const long userId) = 0;

    virtual ~IRepository() {}
};

#endif
