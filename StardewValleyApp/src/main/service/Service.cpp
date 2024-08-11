#include "Service.h"

const vector<Fish> Service::getAllFish(const string& username) const noexcept {
	return fishRepository.findAll(username);
}

string Service::toLowerCase(const string& str) const {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), [](unsigned char c) {
        return tolower(c);
        });
    return lowerStr;
}

const vector<Fish> Service::getAllFishFiltered(const string& username, const string& input) const noexcept {
	vector<Fish> filteredFish;
	vector<Fish> allFish = fishRepository.findAll(username);
    
    string lowerInput = toLowerCase(input);

    auto iterator = std::copy_if(allFish.begin(), allFish.end(), back_inserter(filteredFish), [&](const Fish& fish) {

        if (toLowerCase(fish.getName()).find(lowerInput) != string::npos) {
            return true;
        }

        for (const string& season : fish.getSeason()) {
            if (toLowerCase(season).find(lowerInput) != string::npos) {
                return true;
            }
        }

        for (const string& weather : fish.getWeather()) {
            if (toLowerCase(weather).find(lowerInput) != string::npos) {
                return true;
            }
        }

        for (const string& location : fish.getLocation()) {
            if (toLowerCase(location).find(lowerInput) != string::npos) {
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

const vector<char> Service::getImageByName(const string& name) const {
	return fishRepository.getImageFromImages(name);
}