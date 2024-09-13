#include "Service.h"

const vector<Fish> Service::getAllFish(const long userId) const noexcept {
	return fishRepository.findAll(userId);
}

const long Service::getAllFishNumber() const noexcept {
	return fishRepository.findAllFishNumber();
}

const vector<User> Service::getAllUsers() const noexcept {
	return fishRepository.findAllUsers();
}

const Fish Service::getFishById(const long id, const long userId) const {
	return fishRepository.findOne(id, userId);
}

const vector<Fish> Service::getAllFishFiltered(const long userId, const string& input) const noexcept {
    return fishRepository.findAllFiltered(userId, input);
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

const vector<Fish> Service::getAllFishBySeasonWeatherLocation(const long userId, const string& season, const string& weather, const string& location) const noexcept {
	return fishRepository.findAllBySeasonWeatherLocation(userId, season == "All (No Filter)" ? "" : season, weather == "All (No Filter)" ? "" : weather, location == "All (No Filter)" ? "" : location);
}

const vector<Fish> Service::getAllFishByWeather(const long userId, const string& weather) const noexcept {
	return fishRepository.findAllByWeather(userId, weather);
}

const vector<Fish> Service::getAllFishBySeason(const long userId, const string& season) const noexcept {
	return fishRepository.findAllBySeason(userId, season);
}

const vector<Fish> Service::getAllFishByLocation(const long userId, const string& location) const noexcept {
	return fishRepository.findAllByLocation(userId, location);
}

const vector<Fish> Service::getAllUncaughtFish(const long userId) const noexcept {
	return fishRepository.findAllUncaught(userId);
}

const vector<Fish> Service::getAllFavoriteFish(const long userId) const noexcept {
	return fishRepository.findAllFavorite(userId);
}

const long Service::getCaughtFishNumber(const long userId) const noexcept {
	return fishRepository.getCaughtFishNumber(userId);
}

const long Service::getFavoriteFishNumber(const long userId) const noexcept {
	return fishRepository.getFavoriteFishNumber(userId);
}

const vector<char> Service::getImageByName(const string& name) const {
	return fishRepository.getImageFromImages(name);
}

const QMap<QString, QPixmap> Service::populateImagesCacheMap() const {
	return fishRepository.getAllImages();
}

const Fish Service::updateFish(const Fish& fish, const long userId) const {
	return fishRepository.update(fish, userId);
}