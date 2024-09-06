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
	* @param username - the username of the user
	* @return the fish with the given id
	*/
	const Fish getFishById(const long id, const string& username) const;


	/*
	* Get all fish
	* @param username - the username of the user
	* @return a vector of all the fish
	*/
	const vector<Fish> getAllFish(const string& username) const noexcept;


	/*
	* Get all fish filtered by a given input
	* @param username - the username of the user
	* @param input - the input to filter by
	* @return a vector of all the fish filtered by the input
	*/
	const vector<Fish> getAllFishFiltered(const string& username, const string& input) const noexcept;


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
	* @param username - the username of the user
	* @param season - the season to filter by
	* @param weather - the weather to filter by
	* @param location - the location to filter by
	* @return a vector of all the fish filtered by the season, weather and location
	*/
	const vector<Fish> getAllFishBySeasonWeatherLocation(const string& username, const string& season, const string& weather, const string& location) const noexcept;

	/*
	* Get all fish by weather
	* @param username - the username of the user
	* @param weather - the weather to filter by
	* @return a vector of all the fish filtered by the weather
	*/
	const vector<Fish> getAllFishByWeather(const string& username, const string& weather) const noexcept;


	/*
	* Get all fish by season
	* @param username - the username of the user
	* @param season - the season to filter by
	* @return a vector of all the fish filtered by the season
	*/
	const vector<Fish> getAllFishBySeason(const string& username, const string& season) const noexcept;


	/*
	* Get all fish by location
	* @param username - the username of the user
	* @param location - the location to filter by
	* @return a vector of all the fish filtered by the location
	*/
	const vector<Fish> getAllFishByLocation(const string& username, const string& location) const noexcept;


	/*
	* Get image by name
	* @param name - the name of the image
	* @return a vector of the image
	*/
	const vector<char> getImageByName(const string& name) const;


	/*
	* Add a fish
	* @param fish - the fish to add
	* @param username - the username of the user
	* @return the fish added
	*/
	const Fish updateFish(const Fish& fish, const string& username) const;


	~Service() {}
};