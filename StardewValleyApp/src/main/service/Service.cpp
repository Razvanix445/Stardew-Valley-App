#include "Service.h"

const vector<Fish> Service::getAllFish(const string& username) const noexcept {
	return fishRepository.findAll(username);
}

const Fish Service::getFishById(const long id, const string& username) const {
	return fishRepository.findOne(id, username);
}

const vector<Fish> Service::getAllFishFiltered(const string& username, const string& input) const noexcept {
    return fishRepository.findAllFiltered(username, input);
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

const vector<Fish> Service::getAllFishBySeasonWeatherLocation(const string& username, const string& season, const string& weather, const string& location) const noexcept {
	return fishRepository.findAllBySeasonWeatherLocation(username, season == "All (No Filter)" ? "" : season, weather == "All (No Filter)" ? "" : weather, location == "All (No Filter)" ? "" : location);
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

const vector<Fish> Service::getAllUncaughtFish(const string& username) const noexcept {
	return fishRepository.findAllUncaught(username);
}

const vector<Fish> Service::getAllFavoriteFish(const string& username) const noexcept {
	return fishRepository.findAllFavorite(username);
}

const long Service::getCaughtFishNumber(const string& username) const noexcept {
	return fishRepository.getCaughtFishNumber(username);
}

const vector<char> Service::getImageByName(const string& name) const {
	return fishRepository.getImageFromImages(name);
}

const QMap<QString, QPixmap> Service::populateImagesCacheMap() const {
	return fishRepository.getAllImages();
}

const Fish Service::updateFish(const Fish& fish, const string& username) const {
	return fishRepository.update(fish, username);
}