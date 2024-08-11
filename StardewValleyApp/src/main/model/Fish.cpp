#include "Fish.h"
#include <string>
#include <sstream>

using namespace std;

Fish::Fish() {}
Fish::Fish(const string& name, const string& category, const string& description, const vector<string>& season, const vector<string>& weather, const vector<string>& location, const string& startCatchingHour, const string& endCatchingHour, const long difficulty, const string& movement, const bool isCaught, const std::vector<char>& image) : Entity(0), name(name), category(category), description(description), season(season), weather(weather), location(location), startCatchingHour(startCatchingHour), endCatchingHour(endCatchingHour), difficulty(difficulty), movement(movement), isCaught(isCaught), image(image) {}
Fish::Fish(const long id, const string& name, const string& category, const string& description, const vector<string>& season, const vector<string>& weather, const vector<string>& location, const string& startCatchingHour, const string& endCatchingHour, const long difficulty, const string& movement, const bool isCaught, const std::vector<char>& image) : Entity(id), name(name), category(category), description(description), season(season), weather(weather), location(location), startCatchingHour(startCatchingHour), endCatchingHour(endCatchingHour), difficulty(difficulty), movement(movement), isCaught(isCaught), image(image) {}

const string& Fish::getName() const {
    return this->name;
}

const string& Fish::getCategory() const {
	return this->category;
}

const string& Fish::getDescription() const {
	return this->description;
}

const vector<string>& Fish::getSeason() const {
    return this->season;
}

const vector<string>& Fish::getWeather() const {
    return this->weather;
}

const vector<string>& Fish::getLocation() const {
    return this->location;
}

const string& Fish::getStartCatchingHour() const {
    return this->startCatchingHour;
}

const string& Fish::getEndCatchingHour() const {
    return this->endCatchingHour;
}

long Fish::getDifficulty() const {
    return this->difficulty;
}

const string& Fish::getMovement() const {
	return this->movement;
}

bool Fish::getIsCaught() const {
    return this->isCaught;
}

const std::vector<char>& Fish::getImage() const {
    return this->image;
}

void Fish::setName(const string& name) {
    this->name = name;
}

void Fish::setCategory(const string& category) {
	this->category = category;
}

void Fish::setDescription(const string& description) {
    this->description = description;
}

void Fish::setSeason(const vector<string>& season) {
    this->season = season;
}

void Fish::setWeather(const vector<string>& weather) {
    this->weather = weather;
}

void Fish::setLocation(const vector<string>& location) {
    this->location = location;
}

void Fish::setStartCatchingHour(const string& startCatchingHour) {
    this->startCatchingHour = startCatchingHour;
}

void Fish::setEndCatchingHour(const string& endCatchingHour) {
    this->endCatchingHour = endCatchingHour;
}

void Fish::setDifficulty(const long difficulty) {
    this->difficulty = difficulty;
}

void Fish::setMovement(const string& movement) {
	this->movement = movement;
}

void Fish::setIsCaught(const bool isCaught) {
    this->isCaught = isCaught;
}

void Fish::setImage(const std::vector<char>& image) {
    this->image = image;
}

const string Fish::toString() const {
    std::ostringstream oss;
    oss << "Fish: ";
    oss << "Id: " << id << " ";
    oss << "Name: " << name << " ";
    oss << "No. Seasons: " << season.size() << " ";
    oss << "No. Weathers: " << weather.size() << " ";
    oss << "No. Locations: " << location.size() << " ";
    oss << "Start Catching Hour: " << startCatchingHour << " ";
    oss << "End Cathcing Hour: " << endCatchingHour << " ";
    oss << "Difficulty: " << difficulty << " ";
    oss << "Movement: " << movement << " ";
    oss << "Caught: " << (isCaught ? "Yes" : "No");
    oss << "Has Image: " << (image.size() > 0 ? "Yes" : "No");
    return oss.str();
}
