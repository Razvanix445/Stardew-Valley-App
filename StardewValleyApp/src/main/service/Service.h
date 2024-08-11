#pragma once

#include "../model/Fish.h"
#include "../repository/FishDBRepository.h"
#include <string>

using namespace std;

class Service {

private:
	FishDBRepository& fishRepository;

public:
	
	Service(FishDBRepository& fishRepository) noexcept : fishRepository{ fishRepository } {}

	Service(const Service& other) = delete;



	const vector<Fish> getAllFish(const string& username) const noexcept;

	string toLowerCase(const string& str) const;

	const vector<Fish> getAllFishFiltered(const string& username, const string& input) const noexcept;

	const vector<string> getAllWeathers() const noexcept;

	const vector<string> getAllSeasons() const noexcept;

	const vector<string> getAllLocations() const noexcept;

	const vector<Fish> getAllFishByWeather(const string& username, const string& weather) const noexcept;

	const vector<Fish> getAllFishBySeason(const string& username, const string& season) const noexcept;

	const vector<Fish> getAllFishByLocation(const string& username, const string& location) const noexcept;

	const vector<char> getImageByName(const string& name) const;


	~Service() {}
};