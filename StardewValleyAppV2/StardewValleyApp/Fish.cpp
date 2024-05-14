#include "Fish.h"
#include <string>
#include <sstream>

using namespace std;

Fish::Fish(const string& name, const string& season, const string& weather, const string& location, const long startCatchingHour, const long endCatchingHour, const long difficulty, const bool isCaught) : Entity(0), name(name), season(season), weather(weather), location(location), startCatchingHour(startCatchingHour), endCatchingHour(endCatchingHour), difficulty(difficulty), isCaught(isCaught) {}
Fish::Fish(const long id, const string& name, const string& season, const string& weather, const string& location, const long startCatchingHour, const long endCatchingHour, const long difficulty, const bool isCaught) : Entity(id), name(name), season(season), weather(weather), location(location), startCatchingHour(startCatchingHour), endCatchingHour(endCatchingHour), difficulty(difficulty), isCaught(isCaught) {}

const string& Fish::getName() const {
    return this->name;
}

const string& Fish::getSeason() const {
    return this->season;
}

const string& Fish::getWeather() const {
    return this->weather;
}

const string& Fish::getLocation() const {
    return this->location;
}

long Fish::getStartCatchingHour() const {
    return this->startCatchingHour;
}

long Fish::getEndCatchingHour() const {
    return this->endCatchingHour;
}

long Fish::getDifficulty() const {
    return this->difficulty;
}

bool Fish::getIsCaught() const {
    return this->isCaught;
}

void Fish::setName(const string& name) {
    this->name = name;
}

void Fish::setSeason(const string& season) {
    this->season = season;
}

void Fish::setWeather(const string& weather) {
    this->weather = weather;
}

void Fish::setLocation(const string& location) {
    this->location = location;
}

void Fish::setStartCatchingHour(const long startCatchingHour) {
    this->startCatchingHour = startCatchingHour;
}

void Fish::setEndCatchingHour(const long endCatchingHour) {
    this->endCatchingHour = endCatchingHour;
}

void Fish::setDifficulty(const long difficulty) {
    this->difficulty = difficulty;
}

void Fish::setIsCaught(const bool isCaught) {
    this->isCaught = isCaught;
}

const string Fish::toString() const {
    std::ostringstream oss;
    oss << "Fish: ";
    oss << "Id: " << id << " ";
    oss << "Name: " << name << " ";
    oss << "Season: " << season << " ";
    oss << "Weather: " << weather << " ";
    oss << "Location: " << location << " ";
    oss << "Start Catching Hour: " << startCatchingHour << " ";
    oss << "End Cathcing Hour: " << endCatchingHour << " ";
    oss << "Difficulty: " << difficulty << " ";
    oss << "Caught: " << (isCaught ? "Yes" : "No");
    return oss.str();
}
