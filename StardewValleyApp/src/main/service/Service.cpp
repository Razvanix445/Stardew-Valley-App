#include "Service.h"

const vector<Fish> Service::getAllFish(const string& username) const noexcept {
	return fishRepository.findAll(username);
}

const vector<Fish> Service::getAllFishFiltered(const string& username, const string& input) const noexcept {
	vector<Fish> filteredFish;
	vector<Fish> allFish = fishRepository.findAll(username);
    
    auto iterator = std::copy_if(allFish.begin(), allFish.end(), back_inserter(filteredFish), [&](const Fish& fish) {
        if (fish.getName().find(input) != string::npos) {
            return true;
        }
        for (const string& season : fish.getSeason()) {
            if (season.find(input) != string::npos) {
                return true;
            }
        }
        for (const string& weather : fish.getWeather()) {
            if (weather.find(input) != string::npos) {
                return true;
            }
        }
        for (const string& location : fish.getLocation()) {
            if (location.find(input) != string::npos) {
                return true;
            }
        }
        return false;
        });

    return filteredFish;
}

const vector<string> Service::getAllWeathers() const noexcept {
	return fishRepository.findAllWeathers();
}

const vector<string> Service::getAllSeasons() const noexcept {
	return fishRepository.findAllSeasons();
}

const vector<string> Service::getAllLocations() const noexcept {
	return fishRepository.findAllLocations();
}

const vector<Fish> Service::getAllFishByWeather(const string& username, const string& weather) const noexcept {
	return fishRepository.findAllByWeather(username, weather);
}

const vector<Fish> Service::getAllFishBySeason(const string& username, const string& season) const noexcept {
	return fishRepository.findAllBySeason(username, season);
}

const vector<Fish> Service::getAllFishByLocation(const string& username, const string& location) const noexcept {
	return fishRepository.findAllByLocation(username, location);
}