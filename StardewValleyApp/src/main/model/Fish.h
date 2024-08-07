#ifndef FISH_H
#define FISH_H

#include "Entity.h"
#include <string>
#include <vector>

using namespace std;

class Fish : public Entity {

protected:
    string name;
    vector<string> season;
    vector<string> weather;
    vector<string> location;
    long startCatchingHour;
    long endCatchingHour;
    long difficulty;
    string movement;
    bool isCaught;
    vector<char> image;

public:
    // Constructor
    Fish();
    Fish(const string& name, const vector<string>& season, const vector<string>& weather, const vector<string>& location, const long startCatchingHour, const long endCatchingHour, const long difficulty, const bool isCaught, const std::vector<char>& image);
    Fish(const long id, const string& name, const vector<string>& season, const vector<string>& weather, const vector<string>& location, const long startCatchingHour, const long endCatchingHour, const long difficulty, const bool isCaught, const std::vector<char>& image);

    // Get the Fish name
    const string& getName() const;

    // Get the Fish season(s)
    const vector<string>& getSeason() const;

    // Get the Fish weather(s)
    const vector<string>& getWeather() const;

    // Get the Fish location(s)
    const vector<string>& getLocation() const;

    // Get the Hour from when the Fish can be caught
    long getStartCatchingHour() const;

    // Get the Hour from when the Fish cannot be caught anymore in the current day
    long getEndCatchingHour() const;

    // Get the Fish catching Difficulty
    long getDifficulty() const;

    // Get the Fish movement
    const string& getMovement() const;

    // Get the value 1 if the Fish was already Caught or value 0 if the Fish wasn't Caught yet
    bool getIsCaught() const;

    // Get the Fish image
    const std::vector<char>& getImage() const;

    // Set the Fish name to a new value
    void setName(const string& name);

    // Set the Fish season(s) to a new value
    void setSeason(const vector<string>& season);

    // Set the Fish weather(s) to a new value
    void setWeather(const vector<string>& weather);

    // Set the Fish location(s) to a new value
    void setLocation(const vector<string>& location);

    // Set the Fish starting catching hour to a new value
    void setStartCatchingHour(const long startCatchingHour);

    // Set the Fish ending catching hour to a new value
    void setEndCatchingHour(const long endCatchingHour);

    // Set the Fish catching difficulty to a new value
    void setDifficulty(const long difficulty);

    // Set the Fish movement to a new value
    void setMovement(const string& movement);

    // Set the Fish caught status to a new value
    void setIsCaught(const bool difficulty);

    // Set the Fish image to a new value
    void setImage(const std::vector<char>& image);

    // Convert the Fish object to a string
    const string toString() const;
};

#endif // FISH_H
