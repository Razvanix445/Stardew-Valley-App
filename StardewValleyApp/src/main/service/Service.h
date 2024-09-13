#pragma once

#include "../model/Fish.h"
#include "../repository/FishDBRepository.h"
#include <string>
#include <sstream>

using namespace std;

class Service {

private:
	FishDBRepository& fishRepository;

public:
	
	/*
	* Service class
	* @param fishRepository - the repository of the fish
	*/
	Service(FishDBRepository& fishRepository) noexcept : fishRepository{ fishRepository } {}

	Service(const Service& other) = delete;
	Service(Service&& other) = delete;
	Service() = delete;


	/*
	* Get fish by id
	* @param id - the id of the fish
	* @param userId - the id of the logged user
	* @return the fish with the given id
	*/
	const Fish getFishById(const long id, const long userId) const;


	/*
	* Get all fish
	* @param userId - the id of the logged user
	* @return a vector of all the fish
	*/
	const vector<Fish> getAllFish(const long userId) const noexcept;


	/*
	* Get all fish number
	* @return the number of registered fish in the database
	*/
	const long getAllFishNumber() const noexcept;


	/*
	* Get all users
	* @return a vector of all the users
	*/
	const vector<User> getAllUsers() const noexcept;


	/*
	* Get all fish filtered by a given input
	* @param userId - the id of the logged user
	* @param input - the input to filter by
	* @return a vector of all the fish filtered by the input
	*/
	const vector<Fish> getAllFishFiltered(const long userId, const string& input) const noexcept;


	/*
	* Get all weathers
	* @return a vector of all the weathers
	*/
	const vector<string> getAllWeathers() const noexcept;


	/*
	* Get all seasons
	* @return a vector of all the seasons
	*/
	const vector<string> getAllSeasons() const noexcept;


	/*
	* Get all locations
	* @return a vector of all the locations
	*/
	const vector<string> getAllLocations() const noexcept;


	/*
	* Get all fish by season, weather and location
	* @param userId - the id of the logged user
	* @param season - the season to filter by
	* @param weather - the weather to filter by
	* @param location - the location to filter by
	* @return a vector of all the fish filtered by the season, weather and location
	*/
	const vector<Fish> getAllFishBySeasonWeatherLocation(const long userId, const string& season, const string& weather, const string& location) const noexcept;

	/*
	* Get all fish by weather
	* @param userId - the id of the logged user
	* @param weather - the weather to filter by
	* @return a vector of all the fish filtered by the weather
	*/
	const vector<Fish> getAllFishByWeather(const long userId, const string& weather) const noexcept;


	/*
	* Get all fish by season
	* @param userId - the id of the logged user
	* @param season - the season to filter by
	* @return a vector of all the fish filtered by the season
	*/
	const vector<Fish> getAllFishBySeason(const long userId, const string& season) const noexcept;


	/*
	* Get all fish by location
	* @param userId - the id of the logged user
	* @param location - the location to filter by
	* @return a vector of all the fish filtered by the location
	*/
	const vector<Fish> getAllFishByLocation(const long userId, const string& location) const noexcept;


	/*
	* Get all uncaught fish
	* @param userId - the id of the logged user
	* @return a vector of all the uncaught fish
	*/
	const vector<Fish> getAllUncaughtFish(const long userId) const noexcept;


	/*
	* Get all favorite fish
	* @param userId - the id of the logged user
	* @return a vector of all the favorite fish
	*/
	const vector<Fish> getAllFavoriteFish(const long userId) const noexcept;


	/*
	* Get the number of fish caught by the user
	* @param userId - the id of the logged user
	* @return the number of fish caught by the user
	*/
	const long getCaughtFishNumber(const long userId) const noexcept;


	/*
	* Get the number of fish marked as favorite by the user
	* @param userId - the id of the logged user
	* @return the number of fish marked as favorite by the user
	*/
	const long getFavoriteFishNumber(const long userId) const noexcept;


	/*
	* Get image by name
	* @param name - the name of the image
	* @return a vector of the image
	*/
	const vector<char> getImageByName(const string& name) const;


	/*
	* Populate the QMap with all the images from the database (Images and Fish table)
	* @return a QMap containing all the images
	*/
	const QMap<QString, QPixmap> populateImagesCacheMap() const;


	/*
	* Update a fish
	* @param fish - the fish to update
	* @param userId - the id of the logged user
	* @return the fish updated
	*/
	const Fish updateFish(const Fish& fish, const long userId) const;


	~Service() {}
};