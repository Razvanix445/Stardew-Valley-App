#ifndef FISHDBREPOSITORY_H
#define FISHDBREPOSITORY_H

#include "IRepository.h"
#include "../model/Fish.h"
#include "../../resources/sqlite/sqlite3.h"
#include <QImage>
#include <QPixmap>
#include <QMap>
#include <vector>
#include <qDebug>
#include <algorithm>
#include <string>
#include <iostream>

using namespace std;

class FishDBRepository : public IRepository<Fish> {
private:
    string databasePath;

public:

    /*
    * Default constructor
    */
    FishDBRepository() = default;

    FishDBRepository(const FishDBRepository& other) = delete;

    FishDBRepository(const string& databasePath);


    /*
    * @brief Finds a fish by id
    * @param id - the id of the fish
    * @param username - the username of the user
    * @return the fish with the given id
    */
    Fish findOne(long id, const string& username) const override;


    /*
    * @brief Finds a fish by name
    * @param name - the name of the fish
    * @param username - the username of the user
    * @return the fish with the given name
    */
    Fish findOneByName(const string& name, const string& username) const;


    /*
    * @brief Finds all the fish
    * @param username - the username of the user
    * @return a vector containing all the fish
    */
    vector<Fish> findAll(const string& username) const override;


    /*
    * @brief Saves a fish
    * @param fish - the fish to be saved
    * @return the fish that was saved
    */
    void save(Fish& fish) override;


    /*
    * @brief Removes a fish by id
    * @param id - the id of the fish to be removed
    * @return the fish that was removed
    */
    void remove(long id) override;


    /*
    * @brief Updates a fish
    * @param fish - the fish to be updated
    * @param username - the username of the user
    * @return the fish that was updated
    */
    Fish update(const Fish& fish, const string& username) override;


    /*
    * @brief Saves an image for a fish
    * @param fishId - the id of the fish
    * @param image - the image of the fish
    * @return the image of the fish
    */
    void saveImage(long fishId, const std::vector<char>& image);


    /*
    * @brief Saves an image in the Images table of the database with a name and an image
    * @param name - the name of the fish
    * @param image - the image of the fish
    * @return the image
    */
    void saveImageToImages(const string& name, const std::vector<char>& image);


    /*
    * @brief Gets an image by fish id
    * @param fishId - the id of the fish
    * @return the image of the fish
    */
    std::vector<char> getImage(long fishId) const;


    /*
    * @brief Gets an image by name
    * @param name - the name of the image from the Images table
    * @return the image
    */
    std::vector<char> getImageFromImages(const string& name) const;


    /*
    * @brief Gets all the images from the database to populate the image cache map
    * @return a map containing all the images as a pair of name and image
    */
    QMap<QString, QPixmap> getAllImages() const;


    /*
    * @brief Finds all the weathers
    * @return a vector containing all the weathers
    */
    vector<string> findAllWeathers() const noexcept;


    /*
    * @brief Finds all the seasons
    * @return a vector containing all the seasons
    */
    vector<string> findAllSeasons() const noexcept;


    /*
    * @brief Finds all the locations
    * @return a vector containing all the locations
    */
    vector<string> findAllLocations() const noexcept;


    /*
    * Convert a string to lower case
    * @param str - the string to convert
    * @return the string in lower case
    */
    string toLowerCase(const string& str) const;

    /*
    * @brief Finds all the fish by input
    * @param username - the username of the user
    * @param input - the input to be filtered by
    * @return a vector containing all the fish that contain the input
    */
    vector<Fish> findAllFiltered(const string& username, const string& input) const;


    /*
    * @brief Finds all the fish by season, weather and location
    * @param username - the username of the user
    * @param season - the season of the fish
    * @param weather - the weather of the fish
    * @param location - the location of the fish
    * @return a vector containing all the fish with the given season, weather and location
    */
    vector<Fish> findAllBySeasonWeatherLocation(const string& username, const string& season, const string& weather, const string& location) const noexcept;


    /*
    * @brief Finds all the fish by weather
    * @param username - the username of the user
    * @param weather - the weather of the fish
    * @return a vector containing all the fish with the given weather
    */
    vector<Fish> findAllByWeather(const string& username, const string& weather) const noexcept;


    /*
    * @brief Finds all the fish by season
    * @param username - the username of the user
    * @param season - the season of the fish
    * @return a vector containing all the fish with the given season
    */
    vector<Fish> findAllBySeason(const string& username, const string& season) const noexcept;


    /*
    * @brief Finds all the fish by location
    * @param username - the username of the user
    * @param location - the location of the fish
    * @return a vector containing all the fish with the given location
    */
    vector<Fish> findAllByLocation(const string& username, const string& location) const noexcept;


    /*
    * @brief Finds all the fish that are not caught by the user
    * @param username - the username of the user
    * @return a vector containing all the fish that are not caught by the user
    */
    vector<Fish> findAllUncaught(const string& username) const noexcept;


    /*
    * @brief Finds all the fish that are marked as favorite by the user
    * @param username - the username of the user
    * @return a vector containing all the fish that are marked as favorite by the user
    */
    vector<Fish> findAllFavorite(const string& username) const noexcept;


    /*
    * @brief Finds the number of caught fish by the user
    * @param username - the username of the user
    * @return the number of caught fish by the user
    */
    const long getCaughtFishNumber(const string& username) const noexcept;


    /*
    * @brief Finds the seasons based on the fish id
    * @param db - the database
    * @param fishId - the id of the fish
    * @return a vector containing all the seasons of the fish
    */
    vector<string> getSeasonsByFishId(sqlite3* db, long fishId) const;


    /*
    * @brief Finds the weathers based on the fish id
    * @param db - the database
    * @param fishId - the id of the fish
    * @return a vector containing all the weathers of the fish
    */
    vector<string> getWeathersByFishId(sqlite3* db, long fishId) const;


    /*
    * @brief Finds the locations based on the fish id
    * @param db - the database
    * @param fishId - the id of the fish
    * @return a vector containing all the locations of the fish
    */
    vector<string> getLocationsByFishId(sqlite3* db, long fishId) const;


    /*
    * @brief Checks if the fish is caught by the user based on the fish id
    * @param db - the database
    * @param fishId - the id of the fish
    * @param username - the username of the user
    * @return a bool that is true if the fish is caught by the user and false otherwise
    */
    bool getIsCaughtByFishId(sqlite3* db, long fishId, const string& username) const;


    /*
    * @brief Checks if the fish is favorite by the user based on the fish id
    * @param db - the database
    * @param fishId - the id of the fish
    * @param username - the username of the user
    * @return a bool that is true if the fish is favorite by the user and false otherwise
    */
    bool getIsFavoriteByFishId(sqlite3* db, long fishId, const string& username) const;


    /*
    * @brief Function that modifies tables from the database
    * @param db - the database
    * @param fishName - the name of the fish
    * @param items - the items to be inserted in the table
    * @param tableName - the name of the table
    * @param itemIdColumn - the name of the column that contains the id of the item
    */
    void updateRelatedTable(sqlite3* db, const std::string& fishName, const std::vector<std::string>& items, const std::string& tableName, const std::string& itemIdColumn);


};

#endif // FISHDBREPOSITORY_H