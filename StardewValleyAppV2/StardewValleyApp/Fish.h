#ifndef FISH_H
#define FISH_H

#include "Entity.h"
#include <string>

using namespace std;

class Fish : public Entity {

protected:
    string name;
    string season;
    string weather;
    string location;
    long startCatchingHour;
    long endCatchingHour;
    long difficulty;
    bool isCaught;

public:
    // Constructor
    Fish(const string& name, const string& season, const string& weather, const string& location, const long startCatchingHour, const long endCatchingHour, const long difficulty, const bool isCaught);
    Fish(const long id, const string& name, const string& season, const string& weather, const string& location, const long startCatchingHour, const long endCatchingHour, const long difficulty, const bool isCaught);

    // Get the Fish name
    const string& getName() const;

    // Get the Fish season
    const string& getSeason() const;

    // Get the Fish weather
    const string& getWeather() const;

    // Get the Fish location
    const string& getLocation() const;

    // Get the Hour from when the Fish can be caught
    long getStartCatchingHour() const;

    // Get the Hour from when the Fish cannot be caught anymore in the current day
    long getEndCatchingHour() const;

    // Get the Fish catching Difficulty
    long getDifficulty() const;

    // Get the value 1 if the Fish was already Caught or value 0 if the Fish wasn't Caught yet
    bool getIsCaught() const;

    // Set the Fish name to a new value
    void setName(const string& name);

    // Set the Fish season to a new value
    void setSeason(const string& season);

    // Set the Fish weather to a new value
    void setWeather(const string& weather);

    // Set the Fish location to a new value
    void setLocation(const string& location);

    void setStartCatchingHour(const long startCatchingHour);

    void setEndCatchingHour(const long endCatchingHour);

    void setDifficulty(const long difficulty);

    void setIsCaught(const bool difficulty);

    const string toString() const;
};

#endif // FISH_H
